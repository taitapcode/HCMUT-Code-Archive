#include "tests/TestUtils.h"
#include "BKMove.h"
#include <vector>
#include <string>

static void buildRoute(BusRoute& route,
                       const std::initializer_list<string>& outIds,
                       const std::initializer_list<string>& inIds) {
    SLinkedList<BusStop> out;
    SLinkedList<BusStop> in;
    for (const string& id : outIds) out.add(BusStop(id));
    for (const string& id : inIds) in.add(BusStop(id));
    route.build(out, in);
}

static int journeyIndex(const vector<JourneyResult>& journeys,
                        int transfers,
                        const string& firstRouteId,
                        Direction firstDirection,
                        const string& transferStopId,
                        const string& secondRouteId,
                        Direction secondDirection) {
    for (int i = 0; i < static_cast<int>(journeys.size()); ++i) {
        const JourneyResult& j = journeys[i];
        if (j.transfers == transfers &&
            j.firstRouteId == firstRouteId &&
            j.firstDirection == firstDirection &&
            j.transferStopId == transferStopId &&
            j.secondRouteId == secondRouteId &&
            j.secondDirection == secondDirection) {
            return i;
        }
    }
    return -1;
}

int main(int argc, char* argv[]) {
    PublicTestSuite suite("Q5 - BKMove (Deep Hidden Tests)");

    // =========================================================================
    // Test 1: Transfer where hops2 == 0 is excluded
    // If transferStop IS the destination on route2, hops2=0, NOT included
    // =========================================================================
    suite.add("Transfer hops2==0 excluded", []() {
        // R1: A -> B -> C, R2: X -> C -> Y (C is on R2 but C->C = 0 hops)
        // Query A -> C: direct via R1 (2 hops). Transfer via B? R2 has B? No.
        // Better: R1: A -> B -> C, R2: D -> B -> C
        // Query A -> C: direct R1 = 2 hops.
        // Transfer A->B on R1 (1 hop), then B->C on R2 (1 hop) = valid
        // But if we query A -> B: direct R1 = 1 hop.
        // Transfer via C? R1 A->C = 2 hops. But C==toStopId? No, toStopId=B. 
        // Let's make it clearer:
        // R1: A -> M -> D, R2: X -> M -> D
        // Query A -> M: R1 direct = 1 hop. Transfer A->D on R1 (2 hops) then 
        //   D on R2? But D->M is backward = -1. No transfer via D.
        // What about transfer via M? transferStopId == toStopId = M, excluded. Good.
        BusRoute r1("R1");
        buildRoute(r1, {"A", "M", "D"}, {"D", "M", "A"});
        BusRoute r2("R2");
        buildRoute(r2, {"X", "M", "D"}, {"D", "M", "X"});
        BKMove map;
        map.addRoute(&r1);
        map.addRoute(&r2);

        auto j = map.findJourneys("A", "M");
        // Direct: R1 OUTBOUND A->M = 1 hop. R1 INBOUND M->A not valid.
        // Transfer: A->D on R1 (2 hops), transferStop=D, D->M on R2 INBOUND = 1 hop? 
        //   R2 INBOUND: D -> M -> X. D->M = 1 hop. Valid! Total = 3 hops.
        // Transfer via M: excluded (transferStopId == toStopId)
        // So: 1 direct + 1 transfer = 2
        requireTrue(j.size() >= 1, "has at least direct journey");
        requireEqual(j[0].transfers, 0, "direct first");
        requireEqual(j[0].totalHops, 1, "direct is 1 hop");
        // Verify no transfer uses M as transferStop
        for (const auto& jr : j) {
            if (jr.transfers == 1) {
                requireTrue(jr.transferStopId != "M", "transfer stop cannot be destination");
            }
        }
    });

    // =========================================================================
    // Test 2: fromStopId == toStopId with multiple routes
    // =========================================================================
    suite.add("Same stop query with multiple routes", []() {
        BusRoute r1("R1");
        buildRoute(r1, {"A", "B", "C"}, {"C", "B", "A"});
        BusRoute r2("R2");
        buildRoute(r2, {"X", "A", "Y"}, {"Y", "A", "X"});
        BKMove map;
        map.addRoute(&r1);
        map.addRoute(&r2);

        auto direct = map.findDirectRoutes("A", "A");
        // R1 OUTBOUND: A at index 0, A->A = 0 hops ✓
        // R1 INBOUND: A at index 2, A->A = 0 hops ✓
        // R2 OUTBOUND: A at index 1, A->A = 0 hops ✓
        // R2 INBOUND: A at index 1, A->A = 0 hops ✓
        requireEqual(static_cast<int>(direct.size()), 4, "4 direct results (2 routes x 2 directions)");
        for (const auto& d : direct) {
            requireEqual(d.hopCount, 0, "0 hops for same stop");
        }
    });

    // =========================================================================
    // Test 3: transferStopId == toStopId is excluded
    // =========================================================================
    suite.add("Transfer stop == destination excluded", []() {
        BusRoute r1("R1");
        buildRoute(r1, {"A", "B", "F"}, {"F", "B", "A"});
        BusRoute r2("R2");
        buildRoute(r2, {"X", "F", "Y"}, {"Y", "F", "X"});
        BKMove map;
        map.addRoute(&r1);
        map.addRoute(&r2);

        auto j = map.findJourneys("A", "F");
        // Direct: R1 OUTBOUND A->F = 2 hops
        // Transfer via B: R2 has B? No. No transfer via B.
        // Transfer via F: excluded (F == toStopId)
        // So only 1 direct
        for (const auto& jr : j) {
            if (jr.transfers == 1) {
                requireTrue(jr.transferStopId != "F", "transfer stop not destination");
                requireTrue(jr.transferStopId != "A", "transfer stop not origin");
            }
        }
    });

    // =========================================================================
    // Test 4: Route with both OUTBOUND and INBOUND valid for direct with diff hops
    // =========================================================================
    suite.add("Both directions valid with different hops", []() {
        // OUTBOUND: A -> B -> C -> D -> E  (A->E = 4 hops)
        // INBOUND: E -> X -> A -> Y -> ... hmm, need A in inbound before E's position
        // Let's make: OUTBOUND: A -> B -> C, INBOUND: C -> B -> A
        // Query B -> A: OUTBOUND B(1) -> A(0) = -1 (backward), INBOUND B(1) -> A(2) = 1 hop
        BusRoute r1("R1");
        buildRoute(r1, {"A", "B", "C"}, {"C", "B", "A"});
        BKMove map;
        map.addRoute(&r1);

        // Query A->C: OUTBOUND = 2 hops, INBOUND C(0)->A? No, A->C in inbound: 
        //   INBOUND is C(0), B(1), A(2). A is at index 2, C is at index 0. C before A => C->A = 2 hops but A->C: A(2)->C(0) = -1 backward
        // So only OUTBOUND result
        auto d1 = map.findDirectRoutes("A", "C");
        requireEqual(static_cast<int>(d1.size()), 1, "only outbound");
        requireTrue(d1[0].direction == OUTBOUND, "outbound direction");

        // Query C->A: OUTBOUND C(2)->A(0) = -1. INBOUND C(0)->A(2) = 2 hops.
        auto d2 = map.findDirectRoutes("C", "A");
        requireEqual(static_cast<int>(d2.size()), 1, "only inbound");
        requireTrue(d2[0].direction == INBOUND, "inbound direction");

        // Query A->B: OUTBOUND A(0)->B(1) = 1 hop. INBOUND B(1)->A(2), so A->B means A at idx 2, B at idx 1 => -1 backward
        auto d3 = map.findDirectRoutes("A", "B");
        requireEqual(static_cast<int>(d3.size()), 1, "only outbound A->B");

        // Query B->A: OUTBOUND B(1)->A(0) = -1. INBOUND B(1)->A(2) = 1 hop.
        auto d4 = map.findDirectRoutes("B", "A");
        requireEqual(static_cast<int>(d4.size()), 1, "only inbound B->A");
    });

    // =========================================================================
    // Test 5: Multiple routes with identical stop sequences
    // =========================================================================
    suite.add("Identical stop sequences different IDs", []() {
        BusRoute r1("R1");
        buildRoute(r1, {"A", "B", "C"}, {"C", "B", "A"});
        BusRoute r2("R2");
        buildRoute(r2, {"A", "B", "C"}, {"C", "B", "A"});
        BKMove map;
        map.addRoute(&r1);
        map.addRoute(&r2);

        auto direct = map.findDirectRoutes("A", "C");
        requireEqual(static_cast<int>(direct.size()), 2, "2 routes same hops");
        requireEqual(direct[0].routeId, string("R1"), "R1 before R2 by routeId");
        requireEqual(direct[1].routeId, string("R2"), "R2 after R1");
    });

    // =========================================================================
    // Test 6: Backward direction gives no result
    // =========================================================================
    suite.add("Backward direction returns empty", []() {
        BusRoute r1("R1");
        buildRoute(r1, {"A", "B", "C"}, {"C", "B", "A"});
        BKMove map;
        map.addRoute(&r1);

        // A->C outbound forward, but C->A outbound backward
        // OUTBOUND: C(2) -> A(0): backward = -1
        // INBOUND: C(0) -> A(2): forward = 2 hops => this IS valid!
        auto j = map.findDirectRoutes("C", "A");
        // Actually C->A is valid via INBOUND!
        requireEqual(static_cast<int>(j.size()), 1, "C->A only via inbound");
        requireTrue(j[0].direction == INBOUND, "inbound direction");

        // True backward: query where neither direction works
        // On this route, every pair is reachable in at least one direction
        // Need a route where one stop is outbound-only
        BusRoute r2("R2");
        buildRoute(r2, {"P", "Q", "R"}, {"R", "X", "P"});
        // OUTBOUND: P, Q, R. INBOUND: R, X, P.
        // Q is outbound-only. X is inbound-only.
        // Q->X: OUTBOUND Q present, X absent = -1. INBOUND X(1)->Q? Q absent = -1. 
        BKMove map2;
        map2.addRoute(&r2);
        auto j2 = map2.findDirectRoutes("Q", "X");
        requireEqual(static_cast<int>(j2.size()), 0, "no direct Q->X");
    });

    // =========================================================================
    // Test 7: Empty BKMove
    // =========================================================================
    suite.add("Empty BKMove returns empty vectors", []() {
        BKMove map;
        auto d = map.findDirectRoutes("A", "B");
        auto j = map.findJourneys("A", "B");
        requireTrue(d.empty(), "empty direct");
        requireTrue(j.empty(), "empty journeys");
    });

    // =========================================================================
    // Test 8: Mix of built and unbuilt routes
    // =========================================================================
    suite.add("Mix built and unbuilt routes", []() {
        BusRoute r1("R1");
        buildRoute(r1, {"A", "B", "C"}, {"C", "B", "A"});
        BusRoute r2("R2"); // unbuilt
        BKMove map;
        map.addRoute(&r1);
        map.addRoute(&r2);

        auto j = map.findJourneys("A", "C");
        requireEqual(static_cast<int>(j.size()), 1, "only built route contributes");
        requireEqual(j[0].firstRouteId, string("R1"), "R1 is the only result");
    });

    // =========================================================================
    // Test 9: Single route, no transfer possible (need 2 different routes)
    // =========================================================================
    suite.add("Single route no transfer possible", []() {
        BusRoute r1("R1");
        buildRoute(r1, {"A", "B", "C", "D"}, {"D", "C", "B", "A"});
        BKMove map;
        map.addRoute(&r1);

        auto j = map.findJourneys("A", "D");
        // Only direct: R1 OUTBOUND A->D = 3 hops. No transfer (needs 2 different routes).
        requireEqual(static_cast<int>(j.size()), 1, "only direct, no transfer");
        requireEqual(j[0].transfers, 0, "0 transfers");
    });

    // =========================================================================
    // Test 10: Combinatorial explosion - 3 routes with shared stops
    // =========================================================================
    suite.add("Combinatorial: 3 routes shared stops", []() {
        BusRoute r1("R1");
        buildRoute(r1, {"A", "X", "Y", "B"}, {"B", "Y", "X", "A"});
        BusRoute r2("R2");
        buildRoute(r2, {"P", "X", "Y", "B"}, {"B", "Y", "X", "P"});
        BusRoute r3("R3");
        buildRoute(r3, {"Q", "X", "Y", "B"}, {"B", "Y", "X", "Q"});
        BKMove map;
        map.addRoute(&r1);
        map.addRoute(&r2);
        map.addRoute(&r3);

        auto j = map.findJourneys("A", "B");
        // Direct: R1 OUTBOUND A->B = 3 hops. That's it (R2 and R3 don't have A).
        // Transfer from R1:
        //   via X (1 hop): R2 OUTBOUND X->B = 2 hops (total 3), R2 INBOUND: B(0)->X? X at 2, backward=-1. Hmm wait.
        //     R2 INBOUND: B, Y, X, P. X->B: B at 0, X at 2 => backward. So no.
        //     R3 OUTBOUND X->B = 2 hops (total 3). 
        //   via Y (2 hops): R2 OUTBOUND Y->B = 1 hop (total 3). R3 OUTBOUND Y->B = 1 hop (total 3).
        // So direct (1) + transfers (4) = 5 minimum
        requireTrue(j.size() >= 5, "at least 5 journeys");
        requireEqual(j[0].transfers, 0, "direct first");
    });

    // =========================================================================
    // Test 11: Complete tie-break verification
    // =========================================================================
    suite.add("Full 7-level tie-break", []() {
        // Create a scenario with same totalHops, different firstRouteId
        BusRoute r1("R1");
        buildRoute(r1, {"S", "A", "C", "T"}, {"T", "C", "A", "S"});
        BusRoute r2("R2");
        buildRoute(r2, {"S", "A", "C", "T"}, {"T", "C", "A", "S"});
        BusRoute r3("R3");
        buildRoute(r3, {"X", "C", "F", "Y"}, {"Y", "F", "C", "X"});
        BKMove map;
        map.addRoute(&r1);
        map.addRoute(&r2);
        map.addRoute(&r3);

        auto j = map.findJourneys("A", "F");
        // R1 OUTBOUND A->C (1 hop) then R3 OUTBOUND C->F (1 hop) = total 2
        // R1 INBOUND A->C: INBOUND is T,C,A,S. A at 2, C at 1 => backward=-1.
        //   Wait, INBOUND: T(0), C(1), A(2), S(3). A->C means from=A(2), to=C(1) => -1 backward. No.
        // R2 OUTBOUND A->C (1 hop) then R3 OUTBOUND C->F (1 hop) = total 2
        // Tie-break: transfers(1)==1, totalHops(2)==2, firstRouteId R1 < R2
        int posR1 = journeyIndex(j, 1, "R1", OUTBOUND, "C", "R3", OUTBOUND);
        int posR2 = journeyIndex(j, 1, "R2", OUTBOUND, "C", "R3", OUTBOUND);
        requireTrue(posR1 >= 0, "R1->R3 transfer exists");
        requireTrue(posR2 >= 0, "R2->R3 transfer exists");
        requireTrue(posR1 < posR2, "R1 before R2 by firstRouteId");
    });

    // =========================================================================
    // Test 12: Route added after search
    // =========================================================================
    suite.add("Dynamic route addition", []() {
        BKMove map;
        BusRoute r1("R1");
        buildRoute(r1, {"A", "B", "C"}, {"C", "B", "A"});
        map.addRoute(&r1);

        auto j1 = map.findDirectRoutes("A", "C");
        requireEqual(static_cast<int>(j1.size()), 1, "1 direct before");

        BusRoute r2("R2");
        buildRoute(r2, {"A", "B", "C"}, {"C", "B", "A"});
        map.addRoute(&r2);

        auto j2 = map.findDirectRoutes("A", "C");
        requireEqual(static_cast<int>(j2.size()), 2, "2 directs after adding route");
    });

    // =========================================================================
    // Test 13: Inbound-only reachable transfer
    // =========================================================================
    suite.add("Inbound-only transfer path", []() {
        // R1: outbound P->Q->R, inbound R->A->P. A is inbound-only on R1.
        // R2: outbound X->A->F, inbound F->A->X.
        // Query: looking for transfers involving A
        BusRoute r1("R1");
        buildRoute(r1, {"P", "Q", "R"}, {"R", "A", "P"});
        BusRoute r2("R2");
        buildRoute(r2, {"X", "A", "F"}, {"F", "A", "X"});
        BKMove map;
        map.addRoute(&r1);
        map.addRoute(&r2);

        // Query R -> F: 
        // Direct: R1 has no F, R2 has no R => no direct
        // Transfer: R1 INBOUND R->A (1 hop), then R2 OUTBOUND A->F (2 hops) = total 3
        //   Also R1 INBOUND R->P not useful for R2.
        auto j = map.findJourneys("R", "F");
        requireTrue(!j.empty(), "found transfer journey R->F");
        bool foundInboundTransfer = false;
        for (const auto& jr : j) {
            if (jr.transfers == 1 && jr.firstRouteId == "R1" && jr.firstDirection == INBOUND &&
                jr.transferStopId == "A" && jr.secondRouteId == "R2") {
                foundInboundTransfer = true;
            }
        }
        requireTrue(foundInboundTransfer, "transfer via inbound-only stop A");
    });

    // =========================================================================
    // Test 14: Transfer via stop present in both directions of route2
    // =========================================================================
    suite.add("Transfer to route2 both directions", []() {
        BusRoute r1("R1");
        buildRoute(r1, {"A", "M", "E"}, {"E", "M", "A"});
        BusRoute r2("R2");
        buildRoute(r2, {"X", "M", "F"}, {"F", "M", "X"});
        BKMove map;
        map.addRoute(&r1);
        map.addRoute(&r2);

        // Query A -> F:
        // Direct: none
        // Transfer via M: R1 OUT A->M (1 hop).
        //   R2 OUT M->F (1 hop) ✓ total 2
        //   R2 IN: F(0), M(1), X(2). M->F: M(1), F(0) => backward=-1. No.
        // So only R2 OUTBOUND is valid from M.
        auto j = map.findJourneys("A", "F");
        int posOut = journeyIndex(j, 1, "R1", OUTBOUND, "M", "R2", OUTBOUND);
        requireTrue(posOut >= 0, "transfer via M to R2 OUTBOUND exists");
    });

    // =========================================================================
    // Test 15: Large sorted results verification
    // =========================================================================
    suite.add("Large sorted results (50+ journeys)", []() {
        BKMove map;
        BusRoute* routes[10];
        for (int i = 0; i < 10; ++i) {
            string id = string("R") + (i < 10 ? "0" : "") + to_string(i);
            routes[i] = new BusRoute(id);
            buildRoute(*routes[i], {"START", "MID", "END"}, {"END", "MID", "START"});
            map.addRoute(routes[i]);
        }

        auto j = map.findJourneys("START", "END");
        // Direct: 10 routes x OUTBOUND = 10 direct (INBOUND: END->MID->START, START->END backward=-1)
        // Wait: INBOUND: END(0), MID(1), START(2). START(2)->END(0) = backward. So no INBOUND direct.
        // So 10 direct.
        // Transfer: each route via MID to 9 other routes (only OUTBOUND works from MID->END)
        //   That's 10 * 9 = 90 transfers (but only OUTBOUND of first route has START->MID)
        //   Actually first route INBOUND: END(0), MID(1), START(2). START->MID: backward.
        //   So only OUTBOUND for first leg. 10 * 9 = 90 transfers.
        // Total: 10 + 90 = 100
        requireEqual(static_cast<int>(j.size()), 100, "100 total journeys");

        // Verify sorted order
        for (size_t i = 1; i < j.size(); ++i) {
            const auto& a = j[i - 1];
            const auto& b = j[i];
            bool ordered = false;
            if (a.transfers < b.transfers) ordered = true;
            else if (a.transfers == b.transfers) {
                if (a.totalHops < b.totalHops) ordered = true;
                else if (a.totalHops == b.totalHops) {
                    if (a.firstRouteId < b.firstRouteId) ordered = true;
                    else if (a.firstRouteId == b.firstRouteId) {
                        if (a.firstDirection < b.firstDirection) ordered = true;
                        else if (a.firstDirection == b.firstDirection) {
                            if (a.transferStopId < b.transferStopId) ordered = true;
                            else if (a.transferStopId == b.transferStopId) {
                                if (a.secondRouteId < b.secondRouteId) ordered = true;
                                else if (a.secondRouteId == b.secondRouteId) {
                                    ordered = (a.secondDirection <= b.secondDirection);
                                }
                            }
                        }
                    }
                }
            }
            requireTrue(ordered, "journey ranking correct at index " + to_string(i));
        }

        for (int i = 0; i < 10; ++i) delete routes[i];
    });

    // =========================================================================
    // Test 16: Disconnected subnetworks
    // =========================================================================
    suite.add("Disconnected subnetworks no path", []() {
        BusRoute r1("R1");
        buildRoute(r1, {"A", "B", "C"}, {"C", "B", "A"});
        BusRoute r2("R2");
        buildRoute(r2, {"X", "Y", "Z"}, {"Z", "Y", "X"});
        BKMove map;
        map.addRoute(&r1);
        map.addRoute(&r2);

        requireTrue(map.findDirectRoutes("A", "Z").empty(), "no direct between disconnected");
        requireTrue(map.findJourneys("A", "Z").empty(), "no journey between disconnected");
    });

    return suite.run(argc, argv);
}
