// Extra Q1a cases modelled on what a hidden grader typically checks:
// stale `found` flags, pointer identity/ownership, duplicate keys,
// skewed trees, boundary keys, non-int key types and randomized diffs.
#include "tests/TestUtils.h"
#include "BST.h"

#include <algorithm>
#include <climits>
#include <map>
#include <string>
#include <vector>

namespace {

// Inserts key -> new int(key) so keys and values stay distinguishable.
void insertKeys(BST<int, int*>& tree, const std::vector<int>& keys) {
    for (int key : keys) tree.add(key, new int(key));
}

void checkAllPresent(BST<int, int*>& tree, const std::vector<int>& keys) {
    for (int key : keys) {
        bool found = false;
        int* value = tree.search(key, found);
        CHECK(found);
        CHECK(value != nullptr);
        CHECK(*value == key);
    }
}

// Frees every value the tree hands out, so ownership transfers stay clean.
void drainTree(BST<int, int*>& tree, const std::vector<int>& keys) {
    for (int key : keys) {
        bool ok = false;
        int* value = tree.remove(key, &ok);
        CHECK(ok);
        delete value;
    }
}

int shuffledKey(int index) { return (index * 37 + 11) % 101 - 50; }

}  // namespace

int main(int argc, char* argv[]) {
    PublicTestSuite suite("Q1a - BST extra/hidden-style");

    suite.add("found flag is cleared on a miss after a hit", [] {
        BST<int, int*> tree;
        insertKeys(tree, {5});
        bool found = false;
        CHECK(tree.search(5, found) != nullptr);
        CHECK(found);
        found = true;  // deliberately stale
        CHECK(tree.search(6, found) == nullptr);
        CHECK(!found);
        found = true;
        CHECK(tree.search(-1, found) == nullptr);
        CHECK(!found);
    });

    suite.add("search returns the stored pointer, not a copy", [] {
        BST<int, int*> tree;
        int* payload = new int(1234);
        tree.add(7, payload);
        bool found = false;
        int* value = tree.search(7, found);
        CHECK(found);
        CHECK(value == payload);
        *value = 4321;
        CHECK(*tree.search(7, found) == 4321);
    });

    suite.add("search keeps the tree unchanged", [] {
        BST<int, int*> tree;
        insertKeys(tree, {50, 30, 70, 20, 40, 60, 80});
        const int sizeBefore = tree.size();
        const int heightBefore = tree.height();
        DLinkedList<int*> listBefore = tree.ascendingList();
        for (int key : {50, 30, 70, 20, 40, 60, 80, 0, 99, -5}) {
            bool found = false;
            tree.search(key, found);
        }
        CHECK(tree.size() == sizeBefore);
        CHECK(tree.height() == heightBefore);
        DLinkedList<int*> listAfter = tree.ascendingList();
        CHECK(listAfter.size() == listBefore.size());
        for (int i = 0; i < listAfter.size(); ++i) {
            CHECK(listAfter.get(i) == listBefore.get(i));
        }
    });

    suite.add("null value is found but still null", [] {
        BST<int, int*> tree;
        tree.add(11);  // default value = 0
        bool found = false;
        CHECK(tree.search(11, found) == nullptr);
        CHECK(found);
        found = true;
        CHECK(tree.search(12, found) == nullptr);
        CHECK(!found);
    });

    suite.add("skewed tree built by ascending inserts", [] {
        BST<int, int*> tree;
        std::vector<int> keys;
        for (int key = 0; key < 200; ++key) keys.push_back(key);
        insertKeys(tree, keys);
        CHECK(tree.size() == 200);
        CHECK(tree.height() == 200);
        checkAllPresent(tree, keys);
        for (int key = 200; key < 220; ++key) {
            bool found = true;
            CHECK(tree.search(key, found) == nullptr);
            CHECK(!found);
        }
        DLinkedList<int*> list = tree.ascendingList();
        CHECK(list.size() == 200);
        for (int i = 0; i < list.size(); ++i) CHECK(*list.get(i) == i);
        drainTree(tree, keys);
        CHECK(tree.empty());
    });

    suite.add("skewed tree built by descending inserts", [] {
        BST<int, int*> tree;
        std::vector<int> keys;
        for (int key = 99; key >= 0; --key) keys.push_back(key);
        insertKeys(tree, keys);
        CHECK(tree.size() == 100);
        CHECK(tree.height() == 100);
        checkAllPresent(tree, keys);
        DLinkedList<int*> list = tree.descendingList();
        CHECK(list.size() == 100);
        for (int i = 0; i < list.size(); ++i) CHECK(*list.get(i) == 99 - i);
        drainTree(tree, keys);
    });

    suite.add("boundary and negative keys", [] {
        BST<int, int*> tree;
        std::vector<int> keys = {INT_MIN, INT_MIN + 1, -1, 0, 1, INT_MAX - 1, INT_MAX};
        insertKeys(tree, keys);
        CHECK(tree.size() == static_cast<int>(keys.size()));
        checkAllPresent(tree, keys);
        bool found = true;
        CHECK(tree.search(INT_MIN + 2, found) == nullptr);
        CHECK(!found);
        drainTree(tree, keys);
        CHECK(tree.size() == 0 && tree.height() == 0 && tree.empty());
    });

    suite.add("duplicate insert keeps the first value and the caller's memory", [] {
        BST<int, int*> tree;
        std::vector<int> keys;
        for (int key = 0; key < 32; ++key) keys.push_back(key * 2);
        insertKeys(tree, keys);
        const int heightBefore = tree.height();
        std::vector<std::pair<int, int*>> rejected;
        for (int key = 0; key < 32; ++key) {
            int* unwanted = new int(-key - 1);
            tree.add(key * 2, unwanted);
            rejected.push_back({key * 2, unwanted});
        }
        CHECK(tree.size() == 32);
        CHECK(tree.height() == heightBefore);
        checkAllPresent(tree, keys);  // values are still key, not -key-1
        for (const auto& entry : rejected) {
            CHECK(entry.second != nullptr);
            CHECK(*entry.second == -entry.first / 2 - 1);  // caller still owns it
            delete entry.second;
        }
        drainTree(tree, keys);
    });

    suite.add("remove returns the very pointer that was inserted", [] {
        BST<int, int*> tree;
        std::vector<int*> payloads;
        for (int key : {9, 4, 15, 2, 7, 12, 20}) {
            int* payload = new int(key * 10);
            payloads.push_back(payload);
            tree.add(key, payload);
        }
        int index = 0;
        for (int key : {9, 4, 15, 2, 7, 12, 20}) {
            bool ok = false;
            int* value = tree.remove(key, &ok);
            CHECK(ok);
            CHECK(value == payloads[index]);
            CHECK(*value == key * 10);
            delete value;
            ++index;
            bool found = true;
            CHECK(tree.search(key, found) == nullptr);
            CHECK(!found);
        }
        CHECK(tree.empty() && tree.size() == 0 && tree.height() == 0);
    });

    suite.add("remove of a missing key leaves the tree intact", [] {
        BST<int, int*> tree;
        std::vector<int> keys = {8, 3, 10, 1, 6, 14, 4, 7, 13};
        insertKeys(tree, keys);
        const int heightBefore = tree.height();
        for (int missing : {0, 2, 5, 9, 11, 12, 15, 100, -100}) {
            bool ok = true;
            CHECK(tree.remove(missing, &ok) == nullptr);
            CHECK(!ok);
            CHECK(tree.remove(missing) == nullptr);
        }
        CHECK(tree.size() == static_cast<int>(keys.size()));
        CHECK(tree.height() == heightBefore);
        checkAllPresent(tree, keys);
        drainTree(tree, keys);
    });

    suite.add("removal in shuffled order empties the tree", [] {
        BST<int, int*> tree;
        std::vector<int> keys;
        for (int i = 0; i < 101; ++i) keys.push_back(i - 50);
        insertKeys(tree, keys);
        std::vector<int> order;
        std::vector<int> seen;
        for (int i = 0; i < 101; ++i) {
            int key = shuffledKey(i);
            if (std::find(seen.begin(), seen.end(), key) != seen.end()) continue;
            seen.push_back(key);
            order.push_back(key);
        }
        for (int key : order) {
            bool ok = false;
            int* value = tree.remove(key, &ok);
            CHECK(ok);
            CHECK(value != nullptr && *value == key);
            delete value;
        }
        CHECK(tree.empty());
        CHECK(tree.size() == 0);
        CHECK(tree.height() == 0);
        for (int key : keys) {
            bool found = true;
            CHECK(tree.search(key, found) == nullptr);
            CHECK(!found);
        }
    });

    suite.add("re-insert after removal uses the new value", [] {
        BST<int, int*> tree;
        tree.add(5, new int(50));
        bool ok = false;
        delete tree.remove(5, &ok);
        CHECK(ok);
        CHECK(tree.size() == 0);
        tree.add(5, new int(500));
        CHECK(tree.size() == 1);
        bool found = false;
        int* value = tree.search(5, found);
        CHECK(found && value != nullptr && *value == 500);
        drainTree(tree, {5});
    });

    suite.add("interleaved add/remove matches a reference map", [] {
        BST<int, int*> tree;
        std::map<int, int*> model;
        unsigned seed = 12345u;
        for (int step = 0; step < 3000; ++step) {
            seed = seed * 1103515245u + 12345u;
            const int key = static_cast<int>((seed >> 16) % 60) - 30;
            const int action = static_cast<int>((seed >> 8) % 3);
            if (action < 2) {
                int* value = new int(step);
                tree.add(key, value);
                if (model.count(key) == 0) model[key] = value;
                else delete value;
            } else {
                bool ok = false;
                int* value = tree.remove(key, &ok);
                CHECK(ok == (model.count(key) != 0));
                if (ok) {
                    CHECK(value == model[key]);
                    delete value;
                    model.erase(key);
                }
            }
            CHECK(tree.size() == static_cast<int>(model.size()));
            bool found = true;
            int* value = tree.search(key, found);
            CHECK(found == (model.count(key) != 0));
            if (found) CHECK(value == model[key]);
        }
        std::vector<int> remaining;
        for (const auto& entry : model) remaining.push_back(entry.first);
        drainTree(tree, remaining);
        CHECK(tree.empty() && tree.size() == 0 && tree.height() == 0);
    });

    suite.add("string keys", [] {
        BST<std::string, std::string*> tree;
        std::vector<std::string> keys = {"ben thanh", "ben xe cho lon", "cho lon",
                                         "dai hoc bach khoa", "cong vien tao dan",
                                         "ben xe mien tay", "ben xe mien dong"};
        for (const std::string& key : keys) tree.add(key, new std::string(key));
        CHECK(tree.size() == static_cast<int>(keys.size()));
        for (const std::string& key : keys) {
            bool found = false;
            std::string* value = tree.search(key, found);
            CHECK(found);
            CHECK(value != nullptr);
            CHECK(*value == key);
        }
        bool found = true;
        CHECK(tree.search("ben", found) == nullptr);
        CHECK(!found);
        found = true;
        CHECK(tree.search("ben thanh 2", found) == nullptr);
        CHECK(!found);
        DLinkedList<std::string*> list = tree.ascendingList();
        CHECK(list.size() == static_cast<int>(keys.size()));
        for (int i = 1; i < list.size(); ++i) CHECK(*list.get(i - 1) <= *list.get(i));
        for (const std::string& key : keys) {
            bool ok = false;
            std::string* value = tree.remove(key, &ok);
            CHECK(ok);
            CHECK(value != nullptr && *value == key);
            delete value;
        }
        CHECK(tree.empty());
    });

    suite.add("clear on empty and nonempty trees, then reuse", [] {
        BST<int, int*> tree;
        tree.clear();
        CHECK(tree.empty() && tree.size() == 0 && tree.height() == 0);
        std::vector<int> keys;
        for (int key = 0; key < 40; ++key) keys.push_back(key);
        insertKeys(tree, keys);
        tree.clear();
        CHECK(tree.empty() && tree.size() == 0 && tree.height() == 0);
        for (int key : keys) {
            bool found = true;
            CHECK(tree.search(key, found) == nullptr);
            CHECK(!found);
        }
        tree.add(3, new int(33));
        CHECK(tree.size() == 1 && tree.height() == 1);
        bool found = false;
        int* value = tree.search(3, found);
        CHECK(found && value != nullptr && *value == 33);
        drainTree(tree, {3});
    });

    suite.add("partial removal then clear does not double free", [] {
        BST<int, int*> tree;
        std::vector<int> keys = {50, 25, 75, 10, 40, 60, 90, 5, 15, 35, 45};
        insertKeys(tree, keys);
        for (int key : {25, 75, 5, 90}) {
            bool ok = false;
            int* value = tree.remove(key, &ok);
            CHECK(ok);
            delete value;  // ownership moved out, tree must not delete it again
        }
        CHECK(tree.size() == 7);
        checkAllPresent(tree, {50, 10, 40, 60, 15, 35, 45});
        tree.clear();  // ~Node must only free the remaining values
        CHECK(tree.empty());
    });

    suite.add("two independent trees do not interfere", [] {
        BST<int, int*> first;
        BST<int, int*> second;
        insertKeys(first, {1, 2, 3});
        insertKeys(second, {100, 200});
        CHECK(first.size() == 3 && second.size() == 2);
        bool found = false;
        CHECK(first.search(200, found) == nullptr && !found);
        CHECK(second.search(1, found) == nullptr && !found);
        drainTree(first, {1, 2, 3});
        CHECK(first.empty() && second.size() == 2);
        drainTree(second, {100, 200});
    });

    return suite.run(argc, argv);
}
