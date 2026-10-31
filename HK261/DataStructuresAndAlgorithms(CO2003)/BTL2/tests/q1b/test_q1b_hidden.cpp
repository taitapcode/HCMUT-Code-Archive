// Extra Q1b cases modelled on what a hidden grader typically checks:
// exact post-rotation shapes, height_/bfactor freshness, balance invariant
// after every single operation, duplicate/ownership rules, K = string
// and a randomized differential test against std::set.
#include "tests/TestUtils.h"
#include "AVL.h"

#include <algorithm>
#include <climits>
#include <set>
#include <string>
#include <vector>

namespace {

// Exposes the protected root/count so the tests can verify node invariants.
template <class K, class V>
struct AVLProbe : AVL<K, V> {
    typedef typename BST<K, V>::Node Node;
    Node* root() { return this->pRoot; }
    int nodeCount() { return this->count; }
};

// Returns the height recomputed from structure; records every violation found.
template <class K, class V>
int verify(typename BST<K, V>::Node* node, bool& ok, int& nodes) {
    if (node == 0) return 0;
    ++nodes;
    const int leftHeight = verify<K, V>(node->pLeft, ok, nodes);
    const int rightHeight = verify<K, V>(node->pRight, ok, nodes);

    if (node->height_ != 1 + (leftHeight > rightHeight ? leftHeight : rightHeight)) ok = false;
    if (node->bfactor != rightHeight - leftHeight) ok = false;
    if (node->bfactor < -1 || node->bfactor > 1) ok = false;
    if (node->pLeft != 0 && !(node->pLeft->data.key < node->data.key)) ok = false;
    if (node->pRight != 0 && !(node->data.key < node->pRight->data.key)) ok = false;
    return 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
}

template <class K, class V>
void checkInvariants(AVLProbe<K, V>& tree) {
    bool ok = true;
    int nodes = 0;
    const int height = verify<K, V>(tree.root(), ok, nodes);
    CHECK(ok);
    CHECK(nodes == tree.nodeCount());   // count tracks the real node amount
    CHECK(nodes == tree.size());
    CHECK(height == tree.height()); // height() reads the cached height_
}

std::vector<int> bfsKeys(AVL<int, int*>& tree) {
    std::vector<int> keys;
    DLinkedList<int> list = tree.bfsKey();
    for (int i = 0; i < list.size(); ++i) keys.push_back(list.get(i));
    return keys;
}

std::vector<std::string> bfactorList(AVL<int, int*>& tree) {
    std::vector<std::string> factors;
    DLinkedList<std::string> list = tree.bfactor();
    for (int i = 0; i < list.size(); ++i) factors.push_back(list.get(i));
    return factors;
}

}  // namespace

int main(int argc, char* argv[]) {
    PublicTestSuite suite("Q1b - AVL extra/hidden-style");

    suite.add("empty tree height, size and bfactor", [] {
        AVLProbe<int, int*> tree;
        CHECK(tree.size() == 0 && tree.height() == 0);
        CHECK(tree.bfactor().size() == 0);
        checkInvariants(tree);
        tree.clear();
        CHECK(tree.size() == 0 && tree.height() == 0);
    });

    suite.add("single node is EH with height 1", [] {
        AVLProbe<int, int*> tree;
        tree.add(11, new int(11));
        CHECK(tree.size() == 1 && tree.height() == 1);
        std::vector<std::string> factors = bfactorList(tree);
        CHECK(factors.size() == 1 && factors[0] == "EH");
        checkInvariants(tree);
        bool ok = false;
        delete tree.remove(11, &ok);
        CHECK(ok && tree.empty() && tree.height() == 0);
    });

    suite.add("exact shapes after each rotation type", [] {
        // LL: 30,20,10 -> root 20, left 10, right 30
        {
            AVLProbe<int, int*> tree;
            for (int v : {30, 20, 10}) tree.add(v, new int(v));
            CHECK(tree.height() == 2);
            std::vector<int> keys = bfsKeys(tree);
            CHECK(keys.size() == 3 && keys[0] == 20 && keys[1] == 10 && keys[2] == 30);
            std::vector<std::string> factors = bfactorList(tree);
            CHECK(factors.size() == 3);
            CHECK(factors[0] == "EH" && factors[1] == "EH" && factors[2] == "EH");
            checkInvariants(tree);
        }
        // RR: 10,20,30 -> root 20
        {
            AVLProbe<int, int*> tree;
            for (int v : {10, 20, 30}) tree.add(v, new int(v));
            std::vector<int> keys = bfsKeys(tree);
            CHECK(keys.size() == 3 && keys[0] == 20 && keys[1] == 10 && keys[2] == 30);
            checkInvariants(tree);
        }
        // LR: 30,10,20 -> root 20
        {
            AVLProbe<int, int*> tree;
            for (int v : {30, 10, 20}) tree.add(v, new int(v));
            std::vector<int> keys = bfsKeys(tree);
            CHECK(keys.size() == 3 && keys[0] == 20 && keys[1] == 10 && keys[2] == 30);
            checkInvariants(tree);
        }
        // RL: 10,30,20 -> root 20
        {
            AVLProbe<int, int*> tree;
            for (int v : {10, 30, 20}) tree.add(v, new int(v));
            std::vector<int> keys = bfsKeys(tree);
            CHECK(keys.size() == 3 && keys[0] == 20 && keys[1] == 10 && keys[2] == 30);
            checkInvariants(tree);
        }
    });

    suite.add("bfactor labels for a four level tree", [] {
        AVLProbe<int, int*> tree;
        for (int v : {50, 25, 75, 10, 40, 60, 90}) tree.add(v, new int(v));
        CHECK(tree.size() == 7 && tree.height() == 3);
        std::vector<std::string> factors = bfactorList(tree);
        CHECK(factors.size() == 7);
        CHECK(factors[0] == "EH");   // 50: both subtrees height 2
        CHECK(factors[1] == "EH");   // 25
        CHECK(factors[2] == "EH");   // 75
        checkInvariants(tree);
    });

    suite.add("height stays logarithmic while inserting 1..15", [] {
        AVLProbe<int, int*> tree;
        for (int v = 1; v <= 15; ++v) {
            tree.add(v, new int(v));
            checkInvariants(tree);
        }
        CHECK(tree.size() == 15);
        CHECK(tree.height() == 4);
        std::vector<int> keys = bfsKeys(tree);
        CHECK(keys.size() == 15);
        CHECK(keys[0] == 8);
    });

    suite.add("1000 ascending insertions", [] {
        AVLProbe<int, int*> tree;
        for (int v = 0; v < 1000; ++v) tree.add(v, new int(v));
        CHECK(tree.size() == 1000);
        CHECK(tree.height() >= 10);
        CHECK(tree.height() <= 14);
        checkInvariants(tree);
        for (int v = 0; v < 1000; v += 97) {
            bool found = false;
            int* value = tree.search(v, found);
            CHECK(found && value != nullptr && *value == v);
        }
    });

    suite.add("1000 descending insertions", [] {
        AVLProbe<int, int*> tree;
        for (int v = 999; v >= 0; --v) tree.add(v, new int(v));
        CHECK(tree.size() == 1000);
        CHECK(tree.height() <= 14);
        checkInvariants(tree);
    });

    suite.add("deleting the root with two children rebalances", [] {
        AVLProbe<int, int*> tree;
        for (int v : {50, 25, 75, 10, 40, 60, 90, 5, 15, 35, 45, 55, 65, 85, 95}) {
            tree.add(v, new int(v));
        }
        CHECK(tree.height() == 4);
        for (int v : {50, 25, 75, 10}) {
            bool ok = false;
            int* value = tree.remove(v, &ok);
            CHECK(ok && value != nullptr && *value == v);
            delete value;
            checkInvariants(tree);
        }
        CHECK(tree.size() == 11);
        CHECK(tree.height() <= 4);
    });

    suite.add("delete every node in ascending order", [] {
        AVLProbe<int, int*> tree;
        for (int v = 0; v < 128; ++v) tree.add(v, new int(v));
        for (int v = 0; v < 128; ++v) {
            bool ok = false;
            int* value = tree.remove(v, &ok);
            CHECK(ok && value != nullptr && *value == v);
            delete value;
            checkInvariants(tree);
        }
        CHECK(tree.empty() && tree.size() == 0 && tree.height() == 0);
    });

    suite.add("delete every node in shuffled order", [] {
        AVLProbe<int, int*> tree;
        std::vector<int> order;
        for (int i = 0; i < 101; ++i) {
            int v = (i * 37 + 11) % 101;
            tree.add(v, new int(v));
            order.push_back(v);
        }
        CHECK(tree.size() == 101);
        for (int v : order) {
            bool ok = false;
            int* value = tree.remove(v, &ok);
            CHECK(ok && value != nullptr && *value == v);
            delete value;
            checkInvariants(tree);
        }
        CHECK(tree.empty() && tree.height() == 0);
    });

    suite.add("duplicate insert keeps shape and the caller's value", [] {
        AVLProbe<int, int*> tree;
        for (int v : {10, 20, 30, 40, 50}) tree.add(v, new int(v));
        const int heightBefore = tree.height();
        const std::vector<std::string> factorsBefore = bfactorList(tree);
        std::vector<std::pair<int, int*>> rejected;
        for (int v : {10, 20, 30, 40, 50}) {
            int* unwanted = new int(-v);
            tree.add(v, unwanted);
            rejected.push_back({v, unwanted});
        }
        CHECK(tree.size() == 5);
        CHECK(tree.height() == heightBefore);
        std::vector<std::string> factorsAfter = bfactorList(tree);
        CHECK(factorsAfter.size() == factorsBefore.size());
        for (std::size_t i = 0; i < factorsBefore.size(); ++i) {
            CHECK(factorsAfter[i] == factorsBefore[i]);
        }
        for (const auto& entry : rejected) {
            CHECK(*entry.second == -entry.first);  // untouched, still owned by caller
            delete entry.second;
        }
        checkInvariants(tree);
    });

    suite.add("missing removal leaves the structure untouched", [] {
        AVLProbe<int, int*> tree;
        for (int v : {50, 25, 75, 10, 40, 60, 90}) tree.add(v, new int(v));
        const std::vector<std::string> factorsBefore = bfactorList(tree);
        const std::vector<int> keysBefore = bfsKeys(tree);
        for (int missing : {0, 1, 55, 99, -7}) {
            bool ok = true;
            CHECK(tree.remove(missing, &ok) == nullptr);
            CHECK(!ok);
            CHECK(tree.remove(missing) == nullptr);
        }
        CHECK(tree.size() == 7);
        std::vector<std::string> factorsAfter = bfactorList(tree);
        std::vector<int> keysAfter = bfsKeys(tree);
        CHECK(factorsAfter.size() == factorsBefore.size());
        CHECK(keysAfter.size() == keysBefore.size());
        for (std::size_t i = 0; i < keysBefore.size(); ++i) {
            CHECK(keysAfter[i] == keysBefore[i]);
            CHECK(factorsAfter[i] == factorsBefore[i]);
        }
        checkInvariants(tree);
    });

    suite.add("interleaved add/remove matches std::set", [] {
        AVLProbe<int, int*> tree;
        std::set<int> model;
        unsigned seed = 99991u;
        for (int step = 0; step < 2000; ++step) {
            seed = seed * 1103515245u + 12345u;
            const int key = static_cast<int>((seed >> 16) % 80);
            const int action = static_cast<int>((seed >> 7) % 3);
            if (action < 2) {
                int* value = new int(step);
                tree.add(key, value);
                if (model.count(key) == 0) model.insert(key);
                else delete value;
            } else {
                bool ok = false;
                int* value = tree.remove(key, &ok);
                CHECK(ok == (model.count(key) != 0));
                if (ok) {
                    CHECK(value != nullptr);
                    delete value;
                    model.erase(key);
                }
            }
            CHECK(tree.size() == static_cast<int>(model.size()));
            checkInvariants(tree);
            bool found = true;
            int* value = tree.search(key, found);
            CHECK(found == (model.count(key) != 0));
            CHECK((value != nullptr) == found);
        }
        std::vector<int> remaining(model.begin(), model.end());
        for (int key : remaining) {
            bool ok = false;
            delete tree.remove(key, &ok);
            CHECK(ok);
        }
        CHECK(tree.empty() && tree.height() == 0);
    });

    suite.add("string keys stay balanced", [] {
        AVLProbe<std::string, std::string*> tree;
        std::vector<std::string> keys = {"zulu", "alpha", "mike", "bravo", "yankee",
                                         "charlie", "xray", "delta", "whiskey"};
        for (const std::string& key : keys) tree.add(key, new std::string(key));
        CHECK(tree.size() == static_cast<int>(keys.size()));
        CHECK(tree.height() <= 4);
        checkInvariants(tree);
        for (const std::string& key : keys) {
            bool found = false;
            std::string* value = tree.search(key, found);
            CHECK(found && value != nullptr && *value == key);
        }
        for (const std::string& key : keys) {
            bool ok = false;
            std::string* value = tree.remove(key, &ok);
            CHECK(ok && value != nullptr && *value == key);
            delete value;
            checkInvariants(tree);
        }
        CHECK(tree.empty() && tree.height() == 0);
    });

    suite.add("clear then reuse keeps AVL behaviour", [] {
        AVLProbe<int, int*> tree;
        for (int v = 0; v < 64; ++v) tree.add(v, new int(v));
        tree.clear();
        CHECK(tree.size() == 0 && tree.height() == 0 && tree.empty());
        for (int v = 60; v >= 30; --v) tree.add(v, new int(v));
        CHECK(tree.size() == 31);
        CHECK(tree.height() <= 6);
        checkInvariants(tree);
        for (int v = 30; v <= 60; v += 5) {
            bool found = false;
            CHECK(tree.search(v, found) != nullptr && found);
        }
        for (int v = 30; v <= 60; ++v) {
            bool ok = false;
            delete tree.remove(v, &ok);
            CHECK(ok);
        }
        CHECK(tree.empty());
    });

    return suite.run(argc, argv);
}
