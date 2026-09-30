#include <cassert>
#include <iostream>
#include <string>
#include <vector>

#include "BKMove.h"
#include "tests/TestUtils.h"

static void buildRoute(BusRoute& route,
                       const std::initializer_list<string>& outIds,
                       const std::initializer_list<string>& inIds) {
    SLinkedList<BusStop> out;
    SLinkedList<BusStop> in;
    for (const string& id : outIds) out.add(BusStop(id));
    for (const string& id : inIds) in.add(BusStop(id));
    route.build(out, in);
}

int main(int argc, char* argv[]) {
    PublicTestSuite suite("Q5 - BKMove (Comprehensive & Stress Tests)");

    suite.add("Unbuilt routes and null safety", []() {
        BKMove map;
        BusRoute unbuilt("UNBUILT");
        map.addRoute(&unbuilt);

        requireEqual(map.getRouteCount(), 1, "route count is 1");
        requireTrue(!map.getRoute(0)->isBuilt(), "route is unbuilt");

        auto direct = map.findDirectRoutes("A", "B");
        requireTrue(direct.empty(), "unbuilt route produces no direct routes");

        auto journeys = map.findJourneys("A", "B");
        requireTrue(journeys.empty(), "unbuilt route produces no journeys");
    });

    suite.add("Same stop query (from == to)", []() {
        BusRoute r1("R1");
        buildRoute(r1, {"A", "B", "C"}, {"C", "B", "A"});
        BKMove map;
        map.addRoute(&r1);

        auto direct = map.findDirectRoutes("B", "B");
        requireTrue(!direct.empty(), "same stop direct exists");
        requireEqual(direct[0].hopCount, 0, "same stop has 0 hops");

        auto journeys = map.findJourneys("B", "B");
        requireTrue(!journeys.empty(), "same stop journey exists");
        requireEqual(journeys[0].transfers, 0, "direct journey is first");
        requireEqual(journeys[0].totalHops, 0, "0 total hops");

        // Transfers cannot have transferStopId == fromStopId or transferStopId == toStopId
        for (const auto& j : journeys) {
            if (j.transfers == 1) {
                requireTrue(j.transferStopId != "B", "transfer stop cannot be B");
            }
        }
    });

    suite.add("Multi-route transfer network with multiple transfer options", []() {
        // R1: A -> T1 -> T2 -> T3 -> E
        // R2: X -> T1 -> T2 -> T3 -> B
        BusRoute r1("R1");
        buildRoute(r1, {"A", "T1", "T2", "T3", "E"}, {"E", "T3", "T2", "T1", "A"});
        BusRoute r2("R2");
        buildRoute(r2, {"X", "T1", "T2", "T3", "B"}, {"B", "T3", "T2", "T1", "X"});

        BKMove map;
        map.addRoute(&r1);
        map.addRoute(&r2);

        auto journeys = map.findJourneys("A", "B");
        // All options have transfers = 1
        // Hop breakdown:
        // via T1: hops(A->T1) = 1, hops(T1->B) = 3 => total = 4
        // via T2: hops(A->T2) = 2, hops(T2->B) = 2 => total = 4
        // via T3: hops(A->T3) = 3, hops(T3->B) = 1 => total = 4
        // All have totalHops = 4!
        // Tie-break criterion 5 (transferStopId): T1 < T2 < T3 lexicographically!
        requireTrue(journeys.size() >= 3, "at least 3 transfer journeys");
        requireEqual(journeys[0].transferStopId, string("T1"), "T1 is first by lexicographical stop ID");
        requireEqual(journeys[1].transferStopId, string("T2"), "T2 is second");
        requireEqual(journeys[2].transferStopId, string("T3"), "T3 is third");
    });

    suite.add("Ranking stability with many routes", []() {
        BKMove map;
        BusRoute* routePool[10];
        for (int i = 9; i >= 0; --i) {
            std::string id = "Route_" + std::to_string(i);
            routePool[i] = new BusRoute(id);
            buildRoute(*routePool[i], {"START", "MID", "END"}, {"END", "MID", "START"});
            map.addRoute(routePool[i]);
        }

        auto direct = map.findDirectRoutes("START", "END");
        requireEqual(static_cast<int>(direct.size()), 10, "10 direct routes found");

        // Should be sorted by hopCount (all 2) then routeId: Route_0 .. Route_9
        for (int i = 0; i < 10; ++i) {
            std::string expectedId = "Route_" + std::to_string(i);
            requireEqual(direct[i].routeId, expectedId, "direct sorted by routeId");
        }

        for (int i = 0; i < 10; ++i) {
            delete routePool[i];
        }
    });

    suite.add("Disconnected networks produce no journeys", []() {
        BKMove map;
        BusRoute r1("R1");
        buildRoute(r1, {"A", "B", "C"}, {"C", "B", "A"});
        BusRoute r2("R2");
        buildRoute(r2, {"X", "Y", "Z"}, {"Z", "Y", "X"});
        map.addRoute(&r1);
        map.addRoute(&r2);

        auto direct = map.findDirectRoutes("A", "Z");
        requireTrue(direct.empty(), "disconnected direct is empty");

        auto journeys = map.findJourneys("A", "Z");
        requireTrue(journeys.empty(), "disconnected journey is empty");
    });

    suite.add("Direct journey always precedes transfer journey even with more hops", []() {
        // Direct route: A -> B -> C -> D -> E -> F (5 hops direct)
        BusRoute rDirect("R_DIRECT");
        buildRoute(rDirect, {"A", "B", "C", "D", "E", "F"}, {"F", "E", "D", "C", "B", "A"});

        // Transfer route: A -> M -> F (1 hop on R1, 1 hop on R2 = 2 hops transfer)
        BusRoute rTransfer1("R_T1");
        buildRoute(rTransfer1, {"A", "M"}, {"M", "A"});
        BusRoute rTransfer2("R_T2");
        buildRoute(rTransfer2, {"M", "F"}, {"F", "M"});

        BKMove map;
        map.addRoute(&rDirect);
        map.addRoute(&rTransfer1);
        map.addRoute(&rTransfer2);

        auto journeys = map.findJourneys("A", "F");
        requireTrue(journeys.size() >= 2, "found direct and transfer journeys");
        requireEqual(journeys[0].transfers, 0, "direct journey MUST be first");
        requireEqual(journeys[0].firstRouteId, string("R_DIRECT"), "first journey is R_DIRECT");
        requireEqual(journeys[0].totalHops, 5, "direct journey has 5 hops");

        requireEqual(journeys[1].transfers, 1, "second journey is transfer");
        requireEqual(journeys[1].totalHops, 2, "transfer journey has 2 hops");
    });

    suite.add("Large network stress test (20 routes)", []() {
        BKMove map;
        BusRoute* routes[20];
        for (int i = 0; i < 20; ++i) {
            std::string rId = "Line_" + (i < 10 ? "0" + std::to_string(i) : std::to_string(i));
            routes[i] = new BusRoute(rId);
            // Each line connects Hub_i to Center then to Hub_(i+1)
            std::string hub1 = "Hub_" + std::to_string(i);
            std::string hub2 = "Hub_" + std::to_string((i + 1) % 20);
            buildRoute(*routes[i], {hub1, "Center", hub2}, {hub2, "Center", hub1});
            map.addRoute(routes[i]);
        }

        // Query between Hub_0 and Hub_5 via Center
        auto journeys = map.findJourneys("Hub_0", "Hub_5");
        requireTrue(!journeys.empty(), "found journeys across hubs");
        for (const auto& j : journeys) {
            if (j.transfers == 1) {
                requireEqual(j.transferStopId, string("Center"), "transfer through Center");
            }
        }

        for (int i = 0; i < 20; ++i) {
            delete routes[i];
        }
    });

    return suite.run(argc, argv);
}
