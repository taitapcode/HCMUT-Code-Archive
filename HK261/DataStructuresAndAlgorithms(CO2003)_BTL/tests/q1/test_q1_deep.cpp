#include "SLinkedList.h"
#include "tests/TestUtils.h"
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

// Globals for testing callbacks
static int deleteUserDataCalls = 0;
static void countDelete(SLinkedList<int>*) {
    deleteUserDataCalls++;
}

static bool customIntEqual(int& a, int& b) {
    return (a % 10) == (b % 10);
}

static bool stringEqualIgnoreCase(std::string& a, std::string& b) {
    if (a.length() != b.length()) return false;
    for (size_t i = 0; i < a.length(); ++i) {
        if (tolower(a[i]) != tolower(b[i])) return false;
    }
    return true;
}

static int removeCbCount = 0;
static void countRemoveCb(int) {
    removeCbCount++;
}

int main(int argc, char** argv) {
    PublicTestSuite suite("Q1 - SLinkedList (Deep Hidden Tests)");

    // =========================================================================
    // Test 1: Destructor triggers deleteUserData callback
    // =========================================================================
    suite.add("Destructor triggers deleteUserData callback", []() {
        deleteUserDataCalls = 0;
        {
            SLinkedList<int> list(countDelete);
            list.add(1);
            list.add(2);
            list.add(3);
            // destructor should call countDelete
        }
        requireEqual(deleteUserDataCalls, 1, "destructor calls deleteUserData once");
    });

    // =========================================================================
    // Test 2: Multiple clear() calls on empty and populated list
    // =========================================================================
    suite.add("Multiple clear() calls safe", []() {
        SLinkedList<int> list;
        list.clear();
        requireTrue(list.empty(), "clear on empty list");
        list.clear();
        requireTrue(list.empty(), "double clear on empty list");

        list.add(1);
        list.clear();
        requireTrue(list.empty(), "clear after add");
        list.clear();
        requireTrue(list.empty(), "clear on already-cleared list");
        requireEqual(list.size(), 0, "size is 0");

        // Verify reuse after multiple clears
        list.add(42);
        requireEqual(list.get(0), 42, "reuse after clears");
    });

    // =========================================================================
    // Test 3: Interleaved add and remove sequences
    // =========================================================================
    suite.add("Interleaved add/remove sequences", []() {
        SLinkedList<int> list;
        list.add(10);
        list.add(0, 20); // [20, 10]
        list.add(30);    // [20, 10, 30]

        int r = list.removeAt(1); // removes 10 -> [20, 30]
        requireEqual(r, 10, "removed middle");
        requireEqual(list.size(), 2, "size 2");
        requireEqual(list.get(0), 20, "head is 20");
        requireEqual(list.get(1), 30, "tail is 30");

        list.add(1, 40); // [20, 40, 30]
        list.add(50);    // [20, 40, 30, 50]

        r = list.removeAt(0); // [40, 30, 50]
        requireEqual(r, 20, "removed head");
        r = list.removeAt(2); // [40, 30]
        requireEqual(r, 50, "removed tail");

        requireEqual(list.size(), 2, "size 2 final");
        requireEqual(list.get(0), 40, "head 40");
        requireEqual(list.get(1), 30, "tail 30");

        // Remove all
        list.removeAt(0);
        list.removeAt(0);
        requireTrue(list.empty(), "all removed");

        // Re-add
        list.add(100);
        requireEqual(list.get(0), 100, "re-add after empty");
    });

    // =========================================================================
    // Test 4: removeItem without callback (nullptr)
    // =========================================================================
    suite.add("removeItem without callback", []() {
        SLinkedList<int> list;
        list.add(10);
        list.add(20);
        list.add(30);

        bool removed = list.removeItem(20);
        requireTrue(removed, "20 removed");
        requireEqual(list.size(), 2, "size 2");
        requireEqual(list.get(0), 10, "first");
        requireEqual(list.get(1), 30, "second");

        // Remove non-existent
        removed = list.removeItem(999);
        requireTrue(!removed, "999 not found");
        requireEqual(list.size(), 2, "size unchanged");
    });

    // =========================================================================
    // Test 5: removeItem removes first match only (default + custom equality)
    // =========================================================================
    suite.add("removeItem first match only", []() {
        SLinkedList<int> list;
        list.add(10);
        list.add(20);
        list.add(20);
        list.add(30);

        list.removeItem(20);
        requireEqual(list.size(), 3, "one 20 removed");
        requireEqual(list.get(0), 10, "10 remains");
        requireEqual(list.get(1), 20, "second 20 remains");
        requireEqual(list.get(2), 30, "30 remains");

        // Custom equality: match by last digit
        SLinkedList<int> customList(nullptr, customIntEqual);
        customList.add(11);
        customList.add(25);
        customList.add(35); // matches 25 by last digit (5)
        customList.add(41); // matches 11 by last digit (1)

        customList.removeItem(95); // matches 25 first (last digit 5)
        requireEqual(customList.size(), 3, "one removed");
        requireEqual(customList.get(0), 11, "11 stays");
        requireEqual(customList.get(1), 35, "35 stays (second with digit 5)");
        requireEqual(customList.get(2), 41, "41 stays");
    });

    // =========================================================================
    // Test 6: indexOf and contains on empty list with custom equality
    // =========================================================================
    suite.add("indexOf/contains on empty list with custom equality", []() {
        SLinkedList<int> list(nullptr, customIntEqual);
        requireEqual(list.indexOf(5), -1, "indexOf empty");
        requireTrue(!list.contains(5), "contains empty");

        SLinkedList<std::string> strList(nullptr, stringEqualIgnoreCase);
        std::string s = "Test";
        requireEqual(strList.indexOf(s), -1, "indexOf empty string list");
        requireTrue(!strList.contains(s), "contains empty string list");
    });

    // =========================================================================
    // Test 7: add(0, e) on non-empty list (head insertion)
    // =========================================================================
    suite.add("Head insertion on non-empty list", []() {
        SLinkedList<int> list;
        list.add(10);
        list.add(20);
        list.add(0, 5);
        requireEqual(list.size(), 3, "size 3");
        requireEqual(list.get(0), 5, "new head");
        requireEqual(list.get(1), 10, "shifted");
        requireEqual(list.get(2), 20, "tail");

        // Multiple head insertions
        for (int i = 0; i < 20; ++i) {
            list.add(0, -(i + 1));
        }
        requireEqual(list.size(), 23, "size after many head inserts");
        requireEqual(list.get(0), -20, "newest head");
    });

    // =========================================================================
    // Test 8: removeAt on 2-element list (boundary transitions)
    // =========================================================================
    suite.add("removeAt on 2-element list", []() {
        // Remove head from 2-element
        SLinkedList<int> list;
        list.add(10);
        list.add(20);
        requireEqual(list.removeAt(0), 10, "removed head from 2-elem");
        requireEqual(list.size(), 1, "now 1 elem");
        requireEqual(list.get(0), 20, "remaining is 20");

        // Verify we can add/remove after
        list.add(30);
        requireEqual(list.size(), 2, "back to 2");
        requireEqual(list.get(0), 20, "first");
        requireEqual(list.get(1), 30, "second");

        // Remove tail from 2-element
        requireEqual(list.removeAt(1), 30, "removed tail from 2-elem");
        requireEqual(list.size(), 1, "now 1 elem");
        requireEqual(list.get(0), 20, "remaining is 20");

        // Remove last to empty
        requireEqual(list.removeAt(0), 20, "removed last elem");
        requireTrue(list.empty(), "now empty");

        // Verify list is functional after emptying
        list.add(99);
        requireEqual(list.get(0), 99, "reuse after empty");
    });

    // =========================================================================
    // Test 9: get returns reference - modification through reference
    // =========================================================================
    suite.add("get() returns modifiable reference", []() {
        SLinkedList<int> list;
        list.add(10);
        list.add(20);

        list.get(0) = 15;
        list.get(1) = 25;

        requireEqual(list.get(0), 15, "modified via reference");
        requireEqual(list.get(1), 25, "modified via reference");

        int& ref = list.get(0);
        ref = 100;
        requireEqual(list.get(0), 100, "modified via stored reference");
    });

    // =========================================================================
    // Test 10: Iterator remove
    // =========================================================================
    suite.add("Iterator remove", []() {
        SLinkedList<int> list;
        list.add(10);
        list.add(20);
        list.add(30);

        SLinkedList<int>::Iterator it = list.begin();
        requireEqual(*it, 10, "iterator at head");
        it.remove();
        requireEqual(list.size(), 2, "size 2 after iterator remove");
        requireTrue(!list.contains(10), "10 removed");
        requireTrue(list.contains(20), "20 remains");
        requireTrue(list.contains(30), "30 remains");
    });

    // =========================================================================
    // Test 11: Copy constructor and assignment preserve callbacks
    // =========================================================================
    suite.add("Copy preserves itemEqual callback", []() {
        SLinkedList<int> original(nullptr, customIntEqual);
        original.add(11);
        original.add(25);
        original.add(35);

        SLinkedList<int> copied(original);
        // customIntEqual: match by last digit
        requireEqual(copied.indexOf(45), 1, "copied list uses custom equality (digit 5 matches 25 at idx 1)");
        requireTrue(copied.contains(91), "contains by custom equality (digit 1 matches 11)");

        SLinkedList<int> assigned;
        assigned = original;
        requireEqual(assigned.indexOf(45), 1, "assigned list uses custom equality");
    });

    // =========================================================================
    // Test 12: Negative index handling
    // =========================================================================
    suite.add("Negative index throws out_of_range", []() {
        SLinkedList<int> list;
        list.add(10);

        requireOutOfRange([&]() { list.get(-1); }, "get(-1)");
        requireOutOfRange([&]() { list.get(-100); }, "get(-100)");
        requireOutOfRange([&]() { list.removeAt(-1); }, "removeAt(-1)");
        requireOutOfRange([&]() { list.add(-1, 20); }, "add(-1, e)");
    });

    // =========================================================================
    // Test 13: String type elements
    // =========================================================================
    suite.add("String type elements", []() {
        SLinkedList<std::string> list;
        list.add("hello");
        list.add("world");
        list.add(1, "there");

        requireEqual(list.size(), 3, "size 3");
        requireEqual(list.get(0), std::string("hello"), "idx 0");
        requireEqual(list.get(1), std::string("there"), "idx 1");
        requireEqual(list.get(2), std::string("world"), "idx 2");

        list.removeAt(1);
        requireEqual(list.get(1), std::string("world"), "after remove");
        requireTrue(list.contains(std::string("hello")), "contains hello");
        requireTrue(!list.contains(std::string("there")), "no longer contains there");

        requireEqual(list.toString(), std::string("[hello, world]"), "toString");
    });

    // =========================================================================
    // Test 14: Large alternating insert/remove stress
    // =========================================================================
    suite.add("Large alternating insert/remove stress", []() {
        SLinkedList<int> list;
        for (int i = 0; i < 50; ++i) {
            for (int j = 0; j < 100; ++j) {
                list.add(j);
            }
            for (int j = 0; j < 50; ++j) {
                list.removeAt(0);
            }
            for (int j = 0; j < 50; ++j) {
                list.removeAt(list.size() - 1);
            }
            requireTrue(list.empty(), "empty after cycle " + std::to_string(i));
        }
        requireEqual(list.size(), 0, "final size 0");
    });

    // =========================================================================
    // Test 15: add followed by immediate removeAt of same index in loop
    // =========================================================================
    suite.add("Add then immediate remove in loop", []() {
        SLinkedList<int> list;
        for (int i = 0; i < 1000; ++i) {
            list.add(0, i);
            int r = list.removeAt(0);
            requireEqual(r, i, "removed just-added item");
        }
        requireTrue(list.empty(), "empty after 1000 add-remove cycles");

        list.add(10);
        for (int i = 0; i < 1000; ++i) {
            list.add(1, i);
            int r = list.removeAt(1);
            requireEqual(r, i, "removed just-added at index 1");
        }
        requireEqual(list.size(), 1, "only original item remains");
        requireEqual(list.get(0), 10, "original item is 10");
    });

    // =========================================================================
    // Test 16: deleteUserData called on clear() and destructor
    // =========================================================================
    suite.add("deleteUserData called on both clear and destructor", []() {
        deleteUserDataCalls = 0;
        {
            SLinkedList<int> list(countDelete);
            list.add(1);
            list.add(2);
            list.clear();
            requireEqual(deleteUserDataCalls, 1, "clear calls deleteUserData");
            list.add(3);
        }
        requireEqual(deleteUserDataCalls, 2, "destructor also calls deleteUserData");
    });

    // =========================================================================
    // Test 17: removeItem with callback and without callback
    // =========================================================================
    suite.add("removeItem with and without remove callback", []() {
        SLinkedList<int> list;
        list.add(10);
        list.add(20);
        list.add(30);

        removeCbCount = 0;
        list.removeItem(20, countRemoveCb);
        requireEqual(removeCbCount, 1, "remove callback called");
        requireEqual(list.size(), 2, "size 2");

        // Without callback
        list.removeItem(10);
        requireEqual(removeCbCount, 1, "callback not called without arg");
        requireEqual(list.size(), 1, "size 1");
    });

    return suite.run(argc, argv);
}
