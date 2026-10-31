#include <cassert>
#include <iostream>
#include <string>
#include <vector>

#include "SLinkedList.h"
#include "tests/TestUtils.h"

int main(int argc, char* argv[]) {
    PublicTestSuite suite("Q1 - SLinkedList (Comprehensive & Stress Tests)");

    suite.add("Repeated head and tail insertions/removals", []() {
        SLinkedList<int> list;

        // Insert at head
        for (int i = 0; i < 50; ++i) {
            list.add(0, i);
        }
        requireEqual(list.size(), 50, "size after 50 head insertions");
        requireEqual(list.get(0), 49, "head value check");
        requireEqual(list.get(49), 0, "tail value check");

        // Remove from head
        for (int i = 49; i >= 0; --i) {
            requireEqual(list.removeAt(0), i, "remove head matches");
        }
        requireTrue(list.empty(), "list empty after removing all from head");

        // Insert at tail
        for (int i = 0; i < 50; ++i) {
            list.add(i);
        }
        requireEqual(list.size(), 50, "size after 50 tail insertions");

        // Remove from tail
        for (int i = 49; i >= 0; --i) {
            requireEqual(list.removeAt(list.size() - 1), i, "remove tail matches");
        }
        requireTrue(list.empty(), "list empty after removing all from tail");
    });

    suite.add("Middle insertions and removals", []() {
        SLinkedList<int> list;
        list.add(1);
        list.add(5); // [1, 5]

        list.add(1, 3); // [1, 3, 5]
        list.add(1, 2); // [1, 2, 3, 5]
        list.add(3, 4); // [1, 2, 3, 4, 5]

        requireEqual(list.size(), 5, "size after middle insertions");
        for (int i = 0; i < 5; ++i) {
            requireEqual(list.get(i), i + 1, "elements in order 1..5");
        }

        // Remove middle
        requireEqual(list.removeAt(2), 3, "remove middle element 3");
        requireEqual(list.size(), 4, "size after middle removal");
        requireEqual(list.get(2), 4, "subsequent element shifted left");
    });

    suite.add("State transitions: Empty <-> 1-item cycle", []() {
        SLinkedList<int> list;
        for (int cycle = 0; cycle < 100; ++cycle) {
            requireTrue(list.empty(), "cycle start is empty");
            list.add(cycle);
            requireEqual(list.size(), 1, "size 1 after add");
            requireEqual(list.get(0), cycle, "get single element");
            requireEqual(list.removeAt(0), cycle, "removeAt single element");
            requireTrue(list.empty(), "empty after remove");
        }
    });

    suite.add("Self-assignment and Copy semantics", []() {
        SLinkedList<int> list;
        for (int i = 0; i < 10; ++i) list.add(i * 10);

        // Self assignment
        list = list;
        requireEqual(list.size(), 10, "size unchanged after self assignment");
        for (int i = 0; i < 10; ++i) {
            requireEqual(list.get(i), i * 10, "values unchanged after self assignment");
        }

        // Deep copy check
        SLinkedList<int> copy(list);
        copy.add(999);
        requireEqual(list.size(), 10, "original size unaffected by copy addition");
        requireEqual(copy.size(), 11, "copy size reflects addition");

        SLinkedList<int> assigned;
        assigned = copy;
        copy.clear();
        requireTrue(copy.empty(), "cleared copy is empty");
        requireEqual(assigned.size(), 11, "assigned preserves data after source cleared");
    });

    suite.add("ToString and Iterator traversal", []() {
        SLinkedList<int> list;
        requireEqual(list.toString(), std::string("[]"), "empty list toString");

        list.add(10);
        requireEqual(list.toString(), std::string("[10]"), "single item toString");

        list.add(20);
        list.add(30);
        requireEqual(list.toString(), std::string("[10, 20, 30]"), "multi-item toString");

        // Iterator
        int expected = 10;
        int count = 0;
        for (auto it = list.begin(); it != list.end(); ++it) {
            requireEqual(*it, expected, "iterator value matches");
            expected += 10;
            count++;
        }
        requireEqual(count, 3, "iterator traversed all items");
    });

    suite.add("Large list scale (10,000 items)", []() {
        SLinkedList<int> list;
        const int N = 10000;
        for (int i = 0; i < N; ++i) {
            list.add(i);
        }
        requireEqual(list.size(), N, "size is 10000");
        requireEqual(list.get(0), 0, "first item");
        requireEqual(list.get(N - 1), N - 1, "last item");
        requireEqual(list.indexOf(5000), 5000, "indexOf middle");
        requireEqual(list.indexOf(N + 99), -1, "indexOf non-existent");
        requireTrue(list.contains(9999), "contains last");
        requireTrue(!list.contains(-1), "does not contain -1");

        list.clear();
        requireTrue(list.empty(), "cleared large list is empty");
    });

    return suite.run(argc, argv);
}
