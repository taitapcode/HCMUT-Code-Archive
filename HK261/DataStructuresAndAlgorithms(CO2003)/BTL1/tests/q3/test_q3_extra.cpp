#include <cassert>
#include <iostream>
#include <string>
#include <vector>

#include "BusRoute.h"
#include "tests/TestUtils.h"

static SLinkedList<BusStop> makeStops(const std::initializer_list<std::pair<string, string>>& pairs) {
    SLinkedList<BusStop> stops;
    for (const auto& p : pairs) stops.add(BusStop(p.first, p.second));
    return stops;
}

int main(int argc, char* argv[]) {
    PublicTestSuite suite("Q3 - BusRoute (Comprehensive & Stress Tests)");

    suite.add("Long route with asymmetric stops and shared stops", []() {
        // OUTBOUND: S0 -> S1 -> S2 -> S3 -> S4 -> S5 -> S6 -> S7
        // INBOUND:  S7 -> S8 -> S3 -> S9 -> S1 -> S0
        auto out = makeStops({
            {"S0", "Start"}, {"S1", "Stop1"}, {"S2", "Stop2"}, {"S3", "Central"},
            {"S4", "Stop4"}, {"S5", "Stop5"}, {"S6", "Stop6"}, {"S7", "Term"}
        });
        auto in = makeStops({
            {"S7", "Term"}, {"S8", "Return1"}, {"S3", "Central"},
            {"S9", "Return2"}, {"S1", "Stop1"}, {"S0", "Start"}
        });

        BusRoute route("LINE_50");
        route.build(out, in);

        requireEqual(route.getStopCount(OUTBOUND), 8, "outbound has 8 stops");
        requireEqual(route.getStopCount(INBOUND), 6, "inbound has 6 stops");

        // Outbound checks
        requireEqual(route.getStop(0, OUTBOUND).getId(), string("S0"), "outbound 0 is S0");
        requireEqual(route.getStop(3, OUTBOUND).getId(), string("S3"), "outbound 3 is S3");
        requireEqual(route.getStop(7, OUTBOUND).getId(), string("S7"), "outbound 7 is S7");

        // Inbound checks
        requireEqual(route.getStop(0, INBOUND).getId(), string("S7"), "inbound 0 is S7");
        requireEqual(route.getStop(2, INBOUND).getId(), string("S3"), "inbound 2 is S3 (shared)");
        requireEqual(route.getStop(4, INBOUND).getId(), string("S1"), "inbound 4 is S1 (shared)");
        requireEqual(route.getStop(5, INBOUND).getId(), string("S0"), "inbound 5 is S0");

        // Hop counts
        requireEqual(route.getHopCount("S0", "S7", OUTBOUND), 7, "S0 to S7 outbound = 7 hops");
        requireEqual(route.getHopCount("S1", "S5", OUTBOUND), 4, "S1 to S5 outbound = 4 hops");
        requireEqual(route.getHopCount("S5", "S1", OUTBOUND), -1, "backwards outbound is -1");

        requireEqual(route.getHopCount("S7", "S0", INBOUND), 5, "S7 to S0 inbound = 5 hops");
        requireEqual(route.getHopCount("S8", "S1", INBOUND), 3, "S8 to S1 inbound = 3 hops");
        requireEqual(route.getHopCount("S1", "S8", INBOUND), -1, "backwards inbound is -1");

        // Stops existing in one direction but absent in the other
        requireEqual(route.getHopCount("S0", "S8", OUTBOUND), -1, "S8 absent in outbound");
        requireEqual(route.getHopCount("S4", "S0", INBOUND), -1, "S4 absent in inbound");
    });

    suite.add("Consecutive hops and self hops", []() {
        auto out = makeStops({{"A", "A"}, {"B", "B"}, {"C", "C"}});
        auto in = makeStops({{"C", "C"}, {"D", "D"}, {"A", "A"}});

        BusRoute route("R_TEST");
        route.build(out, in);

        // Self hops for valid stops
        requireEqual(route.getHopCount("A", "A", OUTBOUND), 0, "A to A outbound is 0 hops");
        requireEqual(route.getHopCount("B", "B", OUTBOUND), 0, "B to B outbound is 0 hops");
        requireEqual(route.getHopCount("C", "C", OUTBOUND), 0, "C to C outbound is 0 hops");
        requireEqual(route.getHopCount("D", "D", INBOUND), 0, "D to D inbound is 0 hops");

        // Consecutive hops
        requireEqual(route.getHopCount("A", "B", OUTBOUND), 1, "A to B is 1 hop");
        requireEqual(route.getHopCount("B", "C", OUTBOUND), 1, "B to C is 1 hop");
        requireEqual(route.getHopCount("C", "D", INBOUND), 1, "C to D is 1 hop");
        requireEqual(route.getHopCount("D", "A", INBOUND), 1, "D to A is 1 hop");

        // Non-existent IDs
        requireEqual(route.getHopCount("UNKNOWN", "A", OUTBOUND), -1, "unknown from");
        requireEqual(route.getHopCount("A", "UNKNOWN", OUTBOUND), -1, "unknown to");
        requireEqual(route.getHopCount("UNKNOWN", "UNKNOWN", OUTBOUND), -1, "unknown both");
    });

    suite.add("Boundary exception checking on getStop", []() {
        auto out = makeStops({{"A", "A"}, {"B", "B"}});
        auto in = makeStops({{"B", "B"}, {"A", "A"}});

        BusRoute route("R_BOUND");
        route.build(out, in);

        requireOutOfRange([&]() { route.getStop(-1, OUTBOUND); }, "getStop -1 outbound");
        requireOutOfRange([&]() { route.getStop(-100, INBOUND); }, "getStop -100 inbound");
        requireOutOfRange([&]() { route.getStop(2, OUTBOUND); }, "getStop 2 == count outbound");
        requireOutOfRange([&]() { route.getStop(2, INBOUND); }, "getStop 2 == count inbound");
        requireOutOfRange([&]() { route.getStop(999, OUTBOUND); }, "getStop 999 outbound");
    });

    suite.add("Multiple consecutive rebuilds with varying configurations", []() {
        BusRoute route("R_MULTI");

        // Build 1: 2 stops
        auto out1 = makeStops({{"A", "Alpha"}, {"B", "Beta"}});
        auto in1 = makeStops({{"B", "Beta"}, {"A", "Alpha"}});
        route.build(out1, in1);
        requireEqual(route.getStopCount(OUTBOUND), 2, "build 1 outbound count");
        requireEqual(route.getStopCount(INBOUND), 2, "build 1 inbound count");
        requireEqual(route.getStop(0, OUTBOUND).getName(), string("Alpha"), "build 1 name check");

        // Build 2: 4 stops
        auto out2 = makeStops({{"X1", "X1"}, {"X2", "X2"}, {"X3", "X3"}, {"X4", "X4"}});
        auto in2 = makeStops({{"X4", "X4"}, {"X5", "X5"}, {"X1", "X1"}});
        route.build(out2, in2);
        requireEqual(route.getStopCount(OUTBOUND), 4, "build 2 outbound count");
        requireEqual(route.getStopCount(INBOUND), 3, "build 2 inbound count");
        requireEqual(route.getHopCount("A", "B", OUTBOUND), -1, "old stops removed");
        requireEqual(route.getHopCount("X1", "X4", OUTBOUND), 3, "build 2 hop count");

        // Build 3: 3 stops symmetric
        auto out3 = makeStops({{"M1", "M1"}, {"M2", "M2"}, {"M3", "M3"}});
        auto in3 = makeStops({{"M3", "M3"}, {"M2", "M2"}, {"M1", "M1"}});
        route.build(out3, in3);
        requireEqual(route.getStopCount(OUTBOUND), 3, "build 3 outbound count");
        requireEqual(route.getStopCount(INBOUND), 3, "build 3 inbound count");
        requireEqual(route.getHopCount("M1", "M3", OUTBOUND), 2, "build 3 hop count");
        requireEqual(route.getHopCount("M3", "M1", INBOUND), 2, "build 3 inbound hop count");
    });

    return suite.run(argc, argv);
}
