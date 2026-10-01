#include "tests/TestUtils.h"
#include "BusRoute.h"
#include "SLinkedList.h"
#include <string>
#include <vector>

// Helper to create SLinkedList from vector
SLinkedList<BusStop> makeList(const std::vector<std::string>& ids) {
    SLinkedList<BusStop> list;
    for (const auto& id : ids) {
        list.add(BusStop(id));
    }
    return list;
}

int main(int argc, char* argv[]) {
    PublicTestSuite suite("q3_deep");
    
    // Scenario 1: Inbound-only stops
    suite.add("inbound_only_stops", []() {
        BusRoute route("R1");
        auto out = makeList({"A", "B", "C"});
        auto in = makeList({"C", "X", "Y", "A"});
        route.build(out, in);
        requireEqual(route.getHopCount("A", "X", OUTBOUND), -1, "Querying inbound-only stop in outbound should return -1");
        requireEqual(route.getHopCount("X", "Y", OUTBOUND), -1, "Querying inbound-only stops in outbound should return -1");
    });

    // Scenario 2: Route where outbound and inbound share NO middle stops
    suite.add("no_shared_middle_stops", []() {
        BusRoute route("R2");
        auto out = makeList({"A", "B", "C", "D"});
        auto in = makeList({"D", "E", "F", "A"});
        route.build(out, in);
        requireEqual(route.getStopCount(OUTBOUND), 4, "Outbound count");
        requireEqual(route.getStopCount(INBOUND), 4, "Inbound count");
        requireEqual(route.getStop(1, OUTBOUND).getId(), "B", "Outbound index 1");
        requireEqual(route.getStop(2, OUTBOUND).getId(), "C", "Outbound index 2");
        requireEqual(route.getStop(1, INBOUND).getId(), "E", "Inbound index 1");
        requireEqual(route.getStop(2, INBOUND).getId(), "F", "Inbound index 2");
    });
    
    // Scenario 3: Large route (20+ stops)
    suite.add("large_route", []() {
        BusRoute route("R3");
        SLinkedList<BusStop> out, in;
        for (int i = 0; i <= 25; ++i) out.add(BusStop("S" + std::to_string(i)));
        in.add(BusStop("S25"));
        for (int i = 26; i <= 40; ++i) in.add(BusStop("S" + std::to_string(i)));
        in.add(BusStop("S0"));
        route.build(out, in);
        requireEqual(route.getStopCount(OUTBOUND), 26, "Outbound count");
        requireEqual(route.getStopCount(INBOUND), 17, "Inbound count");
        requireEqual(route.getStop(25, OUTBOUND).getId(), "S25", "Outbound last");
        requireEqual(route.getStop(15, INBOUND).getId(), "S40", "Inbound prev to last");
        requireEqual(route.getStop(16, INBOUND).getId(), "S0", "Inbound last");
    });

    // Scenario 4: Three-stop symmetric route
    suite.add("three_stop_symmetric", []() {
        BusRoute route("R4");
        auto out = makeList({"A", "B", "C"});
        auto in = makeList({"C", "B", "A"});
        route.build(out, in);
        requireEqual(route.getStopCount(INBOUND), 3, "Inbound count");
        requireEqual(route.getStop(0, INBOUND).getId(), "C", "Inbound 0");
        requireEqual(route.getStop(1, INBOUND).getId(), "B", "Inbound 1");
        requireEqual(route.getStop(2, INBOUND).getId(), "A", "Inbound 2");
        requireEqual(route.getHopCount("C", "A", INBOUND), 2, "Hops C->A in inbound");
    });

    // Scenario 5: Route with many inbound-exclusive stops
    suite.add("many_inbound_exclusive", []() {
        BusRoute route("R5");
        auto out = makeList({"A", "B", "C"});
        auto in = makeList({"C", "I1", "I2", "I3", "I4", "I5", "I6", "I7", "I8", "A"});
        route.build(out, in);
        requireEqual(route.getStopCount(OUTBOUND), 3, "Outbound count");
        requireEqual(route.getStopCount(INBOUND), 10, "Inbound count");
        requireEqual(route.getStop(8, INBOUND).getId(), "I8", "Inbound 8");
        requireEqual(route.getHopCount("I1", "I8", INBOUND), 7, "Inbound hops");
    });

    // Scenario 6: getHopCount with reversed order
    suite.add("getHopCount_reversed_order", []() {
        BusRoute route("R6");
        auto out = makeList({"A", "B", "C", "D"});
        auto in = makeList({"D", "C", "B", "A"});
        route.build(out, in);
        requireEqual(route.getHopCount("C", "B", OUTBOUND), -1, "C->B in OUTBOUND is backwards");
        requireEqual(route.getHopCount("B", "C", INBOUND), -1, "B->C in INBOUND is backwards");
    });

    // Scenario 7: getHopCount A->E when E is only in inbound
    suite.add("getHopCount_to_inbound_only", []() {
        BusRoute route("R7");
        auto out = makeList({"A", "B", "C"});
        auto in = makeList({"C", "D", "E", "A"});
        route.build(out, in);
        requireEqual(route.getHopCount("A", "E", OUTBOUND), -1, "E is only in inbound");
    });

    // Scenario 8: Multiple getStop calls in sequence
    suite.add("multiple_getStop_sequence", []() {
        BusRoute route("R8");
        auto out = makeList({"A", "B", "C", "D", "E"});
        auto in = makeList({"E", "F", "G", "H", "A"});
        route.build(out, in);
        requireEqual(route.getStop(2, OUTBOUND).getId(), "C", "out 2");
        requireEqual(route.getStop(1, INBOUND).getId(), "F", "in 1");
        requireEqual(route.getStop(0, OUTBOUND).getId(), "A", "out 0");
        requireEqual(route.getStop(4, INBOUND).getId(), "A", "in 4");
        requireEqual(route.getStop(3, OUTBOUND).getId(), "D", "out 3");
    });

    // Scenario 9: Build with minimum 2-stop lists
    suite.add("minimum_2_stop_lists", []() {
        BusRoute route("R9");
        auto out = makeList({"A", "B"});
        auto in = makeList({"B", "A"});
        route.build(out, in);
        requireEqual(route.getStopCount(OUTBOUND), 2, "out count");
        requireEqual(route.getStopCount(INBOUND), 2, "in count");
        requireEqual(route.getStop(0, OUTBOUND).getId(), "A", "out 0");
        requireEqual(route.getStop(1, OUTBOUND).getId(), "B", "out 1");
        requireEqual(route.getStop(0, INBOUND).getId(), "B", "in 0");
        requireEqual(route.getStop(1, INBOUND).getId(), "A", "in 1");
        requireEqual(route.getHopCount("A", "B", OUTBOUND), 1, "out hops");
        requireEqual(route.getHopCount("B", "A", INBOUND), 1, "in hops");
    });

    // Scenario 10: physicalIndex wrapping for inbound
    suite.add("physicalIndex_wrapping", []() {
        BusRoute route("R10");
        auto out = makeList({"A", "B", "C", "D", "E"});
        auto in = makeList({"E", "X", "Y", "Z", "A"});
        route.build(out, in);
        requireEqual(route.getStop(4, INBOUND).getId(), "A", "Inbound index 4 wraps to A");
        requireEqual(route.getStop(3, INBOUND).getId(), "Z", "Inbound index 3");
    });

    // Scenario 11: getStop on last valid index for both directions
    suite.add("getStop_last_valid_index", []() {
        BusRoute route("R11");
        auto out = makeList({"A", "B", "C", "D"});
        auto in = makeList({"D", "X", "Y", "A"});
        route.build(out, in);
        requireEqual(route.getStop(3, OUTBOUND).getId(), "D", "Last outbound index");
        requireEqual(route.getStop(3, INBOUND).getId(), "A", "Last inbound index");
        requireOutOfRange([&]() { route.getStop(4, OUTBOUND); }, "Out of range outbound");
        requireOutOfRange([&]() { route.getStop(4, INBOUND); }, "Out of range inbound");
    });

    // Scenario 12: Verify getHopCount handles stops that appear in the list but in the wrong direction
    suite.add("getHopCount_wrong_direction", []() {
        BusRoute route("R12");
        auto out = makeList({"A", "B", "C"});
        auto in = makeList({"C", "X", "Y", "A"});
        route.build(out, in);
        requireEqual(route.getHopCount("X", "Y", OUTBOUND), -1, "X->Y only in inbound");
        requireEqual(route.getHopCount("B", "C", INBOUND), -1, "B->C only in outbound");
    });

    return suite.run(argc, argv);
}
