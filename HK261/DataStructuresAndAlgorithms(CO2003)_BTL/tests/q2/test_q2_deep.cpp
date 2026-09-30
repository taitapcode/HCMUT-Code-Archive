#include "tests/TestUtils.h"
#include <string>
#include <sstream>

#define private public
#define protected public
#include "CircularLinkedList.h"
#undef private
#undef protected

int deletedCount = 0;
void deleteItem(CircularLinkedList<int*>* list) {
    if (list) {
        deletedCount++;
    }
}
void deleteItemVal(CircularLinkedList<int>* list) {
    if (list) {
        deletedCount++;
    }
}

bool customEqual(int& a, int& b) {
    return (a % 10) == (b % 10);
}

template<typename T>
void verifyCircularInvariant(PublicTestSuite&, CircularLinkedList<T>& list, const std::string& ctx) {
    if (list.empty()) {
        requireEqual(list.head, nullptr, ctx + " empty head is null");
        requireEqual(list.tail, nullptr, ctx + " empty tail is null");
    } else {
        requireTrue(list.head != nullptr, ctx + " non-empty head not null");
        requireTrue(list.tail != nullptr, ctx + " non-empty tail not null");
        requireEqual(list.tail->next, list.head, ctx + " tail->next == head");
        
        // Traverse to verify count
        int c = 0;
        auto curr = list.head;
        do {
            c++;
            curr = curr->next;
        } while (curr != list.head && c <= list.size() + 1);
        requireEqual(c, list.size(), ctx + " count matches traversal");
    }
}

int main(int argc, char** argv) {
    PublicTestSuite suite("q2_deep");

    suite.add("1. Circular integrity after every operation", [&]() {
        CircularLinkedList<int> list;
        verifyCircularInvariant(suite, list, "init");
        
        list.add(10);
        verifyCircularInvariant(suite, list, "add(10)");
        
        list.add(20);
        verifyCircularInvariant(suite, list, "add(20)");
        
        list.add(0, 5);
        verifyCircularInvariant(suite, list, "add(0, 5)");
        
        list.removeAt(1);
        verifyCircularInvariant(suite, list, "removeAt(1)");
        
        list.removeItem(20);
        verifyCircularInvariant(suite, list, "removeItem(20)");
        
        list.clear();
        verifyCircularInvariant(suite, list, "clear()");
    });

    suite.add("2. Destructor callback", [&]() {
        deletedCount = 0;
        {
            CircularLinkedList<int*> list(deleteItem);
            list.add(new int(1));
            list.add(new int(2));
            list.add(new int(3));
        }
        requireEqual(deletedCount, 1, "Destructor triggered deleteUserData 1 time");
    });

    suite.add("3. removeAt from 2-node list (remove index 0 and index 1)", [&]() {
        CircularLinkedList<int> list1;
        list1.add(10); list1.add(20);
        list1.removeAt(0);
        requireEqual(list1.size(), 1, "size is 1");
        requireEqual(list1.get(0), 20, "get(0) is 20");
        verifyCircularInvariant(suite, list1, "remove 0 from 2-node");

        CircularLinkedList<int> list2;
        list2.add(10); list2.add(20);
        list2.removeAt(1);
        requireEqual(list2.size(), 1, "size is 1");
        requireEqual(list2.get(0), 10, "get(0) is 10");
        verifyCircularInvariant(suite, list2, "remove 1 from 2-node");
    });

    suite.add("4. removeAt from 3-node list (remove middle)", [&]() {
        CircularLinkedList<int> list;
        list.add(10); list.add(20); list.add(30);
        list.removeAt(1);
        requireEqual(list.size(), 2, "size is 2");
        requireEqual(list.get(0), 10, "get(0) is 10");
        requireEqual(list.get(1), 30, "get(1) is 30");
        verifyCircularInvariant(suite, list, "remove middle from 3-node");
    });

    suite.add("5. removeItem with no callback (nullptr)", [&]() {
        CircularLinkedList<int> list;
        list.add(10); list.add(20); list.add(30);
        list.removeItem(20, nullptr);
        requireEqual(list.size(), 2, "size is 2");
        requireEqual(list.get(1), 30, "item removed");
        verifyCircularInvariant(suite, list, "removeItem no callback");
    });

    suite.add("6. removeItem when item is at tail", [&]() {
        CircularLinkedList<int> list;
        list.add(10); list.add(20); list.add(30);
        list.removeItem(30);
        requireEqual(list.size(), 2, "size is 2");
        requireEqual(list.get(1), 20, "item removed at tail");
        verifyCircularInvariant(suite, list, "removeItem at tail");
    });

    suite.add("7. get returns reference - modification through get()", [&]() {
        CircularLinkedList<int> list;
        list.add(10); list.add(20); list.add(30);
        list.get(1) = 25;
        requireEqual(list.get(1), 25, "modified via get()");
        requireEqual(list.indexOf(25), 1, "indexOf finds modified");
    });

    suite.add("8. Interleaved add/remove stress", [&]() {
        CircularLinkedList<int> list;
        for (int i = 0; i < 100; i++) {
            list.add(i);
            list.add(0, i * 2);
            if (i % 3 == 0) list.removeAt(list.size() / 2);
            if (i % 5 == 0) list.removeItem(i);
        }
        verifyCircularInvariant(suite, list, "after interleaved stress");
        requireTrue(list.size() > 0, "list not empty");
    });

    suite.add("9. Copy of empty list", [&]() {
        CircularLinkedList<int> list1;
        CircularLinkedList<int> list2(list1);
        requireEqual(list2.size(), 0, "copied empty size");
        verifyCircularInvariant(suite, list2, "copied empty init");

        CircularLinkedList<int> list3;
        list3.add(1);
        list3 = list1;
        requireEqual(list3.size(), 0, "assigned empty size");
        verifyCircularInvariant(suite, list3, "assigned empty init");
    });

    suite.add("10. Copy constructor preserves callbacks", [&]() {
        CircularLinkedList<int> list1(deleteItemVal, customEqual);
        list1.add(12);
        list1.add(25);
        
        CircularLinkedList<int> list2(list1);
        requireEqual(list2.indexOf(22), 0, "custom equality preserved in copy");
        
        deletedCount = 0;
        list2.clear();
        requireEqual(deletedCount, 1, "deleteUserData preserved in copy");
    });

    suite.add("11. Large list: insert at head repeatedly then verify circular wrap", [&]() {
        CircularLinkedList<int> list;
        for (int i = 0; i < 1000; i++) {
            list.add(0, i);
        }
        verifyCircularInvariant(suite, list, "after 1000 add(0)");
        requireEqual(list.size(), 1000, "size 1000");
        requireEqual(list.get(0), 999, "head is 999");
        requireEqual(list.get(999), 0, "tail is 0");
    });

    suite.add("12. String type elements", [&]() {
        CircularLinkedList<std::string> list;
        list.add("hello");
        list.add("world");
        list.add(1, "there");
        requireEqual(list.get(0), std::string("hello"), "str 0");
        requireEqual(list.get(1), std::string("there"), "str 1");
        requireEqual(list.get(2), std::string("world"), "str 2");
        list.removeAt(1);
        requireEqual(list.size(), 2, "str size");
        verifyCircularInvariant(suite, list, "string list");
    });

    suite.add("13. clear() on empty list multiple times", [&]() {
        CircularLinkedList<int> list;
        list.clear();
        list.clear();
        list.clear();
        requireEqual(list.size(), 0, "still 0");
        verifyCircularInvariant(suite, list, "cleared empty");
    });

    suite.add("14. add(index=count) is equivalent to add(T)", [&]() {
        CircularLinkedList<int> list1;
        list1.add(10);
        list1.add(1, 20);
        list1.add(2, 30);
        
        CircularLinkedList<int> list2;
        list2.add(10);
        list2.add(20);
        list2.add(30);
        
        requireEqual(list1.size(), list2.size(), "sizes match");
        requireEqual(list1.get(2), list2.get(2), "elements match");
        verifyCircularInvariant(suite, list1, "add at count");
    });

    suite.add("15. indexOf on non-existent item in large list", [&]() {
        CircularLinkedList<int> list;
        for (int i = 0; i < 100; i++) {
            list.add(i);
        }
        int idx = list.indexOf(999);
        requireEqual(idx, -1, "indexOf non-existent returns -1 without infinite loop");
    });

    return suite.run(argc, argv);
}
