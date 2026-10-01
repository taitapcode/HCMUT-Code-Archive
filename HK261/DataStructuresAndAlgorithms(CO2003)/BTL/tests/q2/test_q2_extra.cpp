#include <cassert>
#include <iostream>
#include <string>
#include <vector>

#include "CircularLinkedList.h"
#include "tests/TestUtils.h"

int main(int argc, char* argv[]) {
    PublicTestSuite suite("Q2 - CircularLinkedList (Comprehensive & Stress Tests)");

    suite.add("Repeated cycle: Empty <-> 1-node <-> 2-node", []() {
        CircularLinkedList<int> list;

        for (int cycle = 0; cycle < 50; ++cycle) {
            requireTrue(list.empty(), "must be empty at cycle start");

            // 0 -> 1 node
            list.add(100 + cycle);
            requireEqual(list.size(), 1, "size 1");
            requireEqual(list.get(0), 100 + cycle, "get single");

            // 1 -> 2 nodes
            list.add(200 + cycle);
            requireEqual(list.size(), 2, "size 2");
            requireEqual(list.get(0), 100 + cycle, "get head of 2");
            requireEqual(list.get(1), 200 + cycle, "get tail of 2");

            // Remove tail -> 1 node
            requireEqual(list.removeAt(1), 200 + cycle, "remove tail");
            requireEqual(list.size(), 1, "size 1 after removing tail");
            requireEqual(list.get(0), 100 + cycle, "remaining item");

            // Remove head -> empty
            requireEqual(list.removeAt(0), 100 + cycle, "remove head");
            requireTrue(list.empty(), "empty after removing head");
        }
    });

    suite.add("Add at index 0 and index == size repeatedly", []() {
        CircularLinkedList<int> list;

        // Prepend 30 items
        for (int i = 0; i < 30; ++i) {
            list.add(0, i);
        }
        requireEqual(list.size(), 30, "size after prepends");
        requireEqual(list.get(0), 29, "head item");
        requireEqual(list.get(29), 0, "tail item");

        // Append 30 items via add(size(), val)
        for (int i = 0; i < 30; ++i) {
            list.add(list.size(), 100 + i);
        }
        requireEqual(list.size(), 60, "size after appends");
        requireEqual(list.get(30), 100, "item at index 30");
        requireEqual(list.get(59), 129, "item at index 59");

        // Clear and reuse
        list.clear();
        requireTrue(list.empty(), "empty after clear");
        list.add(777);
        requireEqual(list.get(0), 777, "reuse after clear");
    });

    suite.add("Middle insertion and removal sequence", []() {
        CircularLinkedList<int> list;
        list.add(10);
        list.add(40); // [10, 40]

        list.add(1, 20); // [10, 20, 40]
        list.add(2, 30); // [10, 20, 30, 40]

        requireEqual(list.size(), 4, "size after 4 items");
        for (int i = 0; i < 4; ++i) {
            requireEqual(list.get(i), (i + 1) * 10, "items in step of 10");
        }

        // Remove from index 1 (middle)
        requireEqual(list.removeAt(1), 20, "remove 20");
        requireEqual(list.size(), 3, "size 3");
        requireEqual(list.get(1), 30, "new index 1 is 30");

        // Remove from index 1 again
        requireEqual(list.removeAt(1), 30, "remove 30");
        requireEqual(list.size(), 2, "size 2");
        requireEqual(list.get(0), 10, "index 0 is 10");
        requireEqual(list.get(1), 40, "index 1 is 40");
    });

    suite.add("Self-assignment and Copy integrity", []() {
        CircularLinkedList<int> list;
        for (int i = 1; i <= 5; ++i) list.add(i * 11);

        // Self assignment
        list = list;
        requireEqual(list.size(), 5, "size unchanged after self assignment");
        requireEqual(list.get(0), 11, "val 0 unchanged");
        requireEqual(list.get(4), 55, "val 4 unchanged");

        // Copy constructor
        CircularLinkedList<int> copied(list);
        copied.add(0, 999);
        requireEqual(list.size(), 5, "original list unmodified");
        requireEqual(copied.size(), 6, "copied list has 6 items");
        requireEqual(copied.get(0), 999, "copied list has new head");
        requireEqual(copied.get(1), 11, "copied list preserved subsequent items");

        // Copy assignment
        CircularLinkedList<int> assigned;
        assigned = copied;
        copied.clear();
        requireTrue(copied.empty(), "copied cleared");
        requireEqual(assigned.size(), 6, "assigned kept items");
    });

    suite.add("ToString and Search boundaries", []() {
        CircularLinkedList<int> list;
        requireEqual(list.toString(), std::string("[]"), "empty list toString");

        list.add(5);
        requireEqual(list.toString(), std::string("[5]"), "1-item toString");

        list.add(15);
        list.add(25);
        requireEqual(list.toString(), std::string("[5, 15, 25]"), "3-item toString");

        requireEqual(list.indexOf(5), 0, "indexOf head");
        requireEqual(list.indexOf(15), 1, "indexOf mid");
        requireEqual(list.indexOf(25), 2, "indexOf tail");
        requireEqual(list.indexOf(999), -1, "indexOf missing");
        requireTrue(list.contains(15), "contains 15");
        requireTrue(!list.contains(999), "does not contain 999");
    });

    suite.add("Scale test (5,000 items)", []() {
        CircularLinkedList<int> list;
        const int N = 5000;
        for (int i = 0; i < N; ++i) {
            list.add(i);
        }
        requireEqual(list.size(), N, "size 5000");
        requireEqual(list.get(0), 0, "first item");
        requireEqual(list.get(N - 1), N - 1, "last item");
        requireEqual(list.indexOf(2500), 2500, "indexOf mid");

        // Remove half from tail
        for (int i = 0; i < N / 2; ++i) {
            list.removeAt(list.size() - 1);
        }
        requireEqual(list.size(), N / 2, "size after removing half");
        requireEqual(list.get(list.size() - 1), (N / 2) - 1, "new tail");

        list.clear();
        requireTrue(list.empty(), "empty after clear");
    });

    return suite.run(argc, argv);
}
