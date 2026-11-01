// Extra Q2 cases modelled on what a hidden grader typically checks:
// underflow behaviour, comparator-driven ordering, O(log m) push/pop,
// heap-array invariants, duplicate removal, remove-with-callback,
// heapify (linear, replaces contents, grows) and memory ownership.
#include "tests/TestUtils.h"
#include "Heap.h"

#include <algorithm>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

int byDescending(int& a, int& b) { return (b > a) - (b < a); }

// Comparator that counts how many times the heap consults it.
int countedCompares = 0;
int countingAscending(int& a, int& b) {
    ++countedCompares;
    return (a > b) - (a < b);
}

struct Task {
    int priority;
    int sequence;
};
// Heap<T>::compare() still compiles its `a < b` fallback, so T needs the
// operators even when a comparator is supplied.
bool operator<(const Task& a, const Task& b) { return a.priority < b.priority; }
bool operator>(const Task& a, const Task& b) { return a.priority > b.priority; }
// toString() also compiles its `os << elements[idx]` fallback.
std::ostream& operator<<(std::ostream& os, const Task& t) { return os << t.priority << '/' << t.sequence; }

int byPriorityThenSequence(Task& a, Task& b) {
    if (a.priority != b.priority) return (a.priority > b.priority) - (a.priority < b.priority);
    return (a.sequence > b.sequence) - (a.sequence < b.sequence);
}

struct Payload {
    static int live;
    int value;
    explicit Payload(int v) : value(v) { ++live; }
    ~Payload() { --live; }
};
int Payload::live = 0;

int byPayloadValue(Payload*& a, Payload*& b) { return (a->value > b->value) - (a->value < b->value); }

int dropCalls = 0;
void dropPayload(Payload* p) {
    ++dropCalls;
    delete p;
}

int hitCalls = 0;
void countHit(int) { ++hitCalls; }

// Reaches Heap's protected array so the heap-order invariant can be checked on
// the raw layout, not only through pop() order.
struct HeapProbe : Heap<int> {
    int capacityOf() { return capacity; }
    int countOf() { return count; }
    bool validHeapArray() {
        if (count < 0 || count > capacity) return false;
        for (int i = 1; i < count; ++i) {
            if (elements[(i - 1) / 2] > elements[i]) return false;
        }
        return true;
    }
};

unsigned nextRandom(unsigned& seed) {
    seed = seed * 1103515245u + 12345u;
    return seed >> 8;
}

}  // namespace

int main(int argc, char* argv[]) {
    PublicTestSuite suite("Q2 - Binary Heap extra/hidden-style");

    suite.add("underflow on empty heap leaves it usable", [] {
        Heap<int> heap;
        bool thrown = false;
        try { heap.peek(); } catch (const std::underflow_error&) { thrown = true; }
        CHECK(thrown);
        thrown = false;
        try { heap.pop(); } catch (const std::underflow_error&) { thrown = true; }
        CHECK(thrown);
        CHECK(heap.size() == 0 && heap.empty());
        heap.push(4);
        CHECK(heap.size() == 1 && heap.peek() == 4);
        CHECK(heap.pop() == 4 && heap.empty());
    });

    suite.add("default comparator pops shuffled input in ascending order", [] {
        constexpr int N = 500;
        std::vector<int> values;
        unsigned seed = 7u;
        for (int i = 0; i < N; ++i) values.push_back(static_cast<int>(nextRandom(seed) % 1000) - 500);
        std::vector<int> sorted = values;
        std::sort(sorted.begin(), sorted.end());
        Heap<int> heap;
        for (int value : values) heap.push(value);
        CHECK(heap.size() == N);
        for (int i = 0; i < N; ++i) CHECK(heap.pop() == sorted[i]);
        CHECK(heap.empty());
    });

    suite.add("custom comparator orders by priority then tie-break field", [] {
        Heap<int> maxHeap(byDescending);
        for (int value : {3, 3, 9, 1, 9, 5}) maxHeap.push(value);
        CHECK(maxHeap.pop() == 9 && maxHeap.pop() == 9 && maxHeap.pop() == 5);
        CHECK(maxHeap.pop() == 3 && maxHeap.pop() == 3 && maxHeap.pop() == 1);

        Heap<Task> tasks(byPriorityThenSequence);
        for (int i = 0; i < 12; ++i) tasks.push(Task{i % 3, i});
        CHECK(tasks.size() == 12);
        int previousPriority = -1;
        int previousSequence = -1;
        for (int i = 0; i < 12; ++i) {
            Task task = tasks.pop();
            CHECK(task.priority > previousPriority ||
                  (task.priority == previousPriority && task.sequence > previousSequence));
            previousPriority = task.priority;
            previousSequence = task.sequence;
        }
    });

    suite.add("push and pop stay logarithmic in comparisons", [] {
        constexpr int N = 1024;
        std::vector<int> values;
        unsigned seed = 99u;
        for (int i = 0; i < N; ++i) values.push_back(static_cast<int>(nextRandom(seed) % 10000));

        countedCompares = 0;
        Heap<int> heap(countingAscending);
        for (int value : values) heap.push(value);
        CHECK(heap.size() == N);
        CHECK(countedCompares < 12 * N);  // a linear reheapUp would be ~N^2/2
        countedCompares = 0;
        for (int i = 0; i < N; ++i) heap.pop();
        CHECK(countedCompares < 25 * N);  // two compares per level, ~2*log2(N) per pop
        CHECK(heap.empty());
    });

    suite.add("heap order holds across the whole array after mixed operations", [] {
        HeapProbe heap;
        unsigned seed = 2024u;
        for (int step = 0; step < 600; ++step) {
            const unsigned action = nextRandom(seed) % 10;
            const int value = static_cast<int>(nextRandom(seed) % 200) - 100;
            if (action < 6) {
                heap.push(value);
            } else if (action < 8) {
                heap.remove(value);
            } else if (!heap.empty()) {
                heap.pop();
            }
            CHECK(heap.validHeapArray());
            CHECK(heap.countOf() == heap.size());
            CHECK(heap.countOf() <= heap.capacityOf());
        }
        int previous = heap.empty() ? 0 : heap.peek();
        while (!heap.empty()) {
            const int current = heap.pop();
            CHECK(current >= previous);
            previous = current;
            CHECK(heap.validHeapArray());
        }
    });

    suite.add("remove drops exactly one copy of a duplicated value", [] {
        Heap<int> heap;
        for (int value : {5, 5, 5, 1, 9, 5}) heap.push(value);
        CHECK(heap.size() == 6);
        heap.remove(5);
        CHECK(heap.size() == 5);
        CHECK(heap.contains(5));
        int fives = 0;
        std::vector<int> rest;
        while (!heap.empty()) {
            const int value = heap.pop();
            if (value == 5) ++fives;
            else rest.push_back(value);
        }
        CHECK(fives == 3);
        CHECK(rest.size() == 2 && rest[0] == 1 && rest[1] == 9);
    });

    suite.add("remove of an interior item whose replacement must bubble up", [] {
        // Pushing in this order reproduces the valid heap array
        // [1,2,10,50,3,40,41,51,52,4,5] exactly, so removing 40 (index 5) puts 5
        // under parent 10: only reheapUp can repair that.
        HeapProbe heap;
        for (int value : {1, 2, 10, 50, 3, 40, 41, 51, 52, 4, 5}) heap.push(value);
        CHECK(heap.validHeapArray());
        heap.remove(40);
        CHECK(heap.size() == 10);
        CHECK(heap.validHeapArray());
        std::vector<int> remaining;
        while (!heap.empty()) remaining.push_back(heap.pop());
        const std::vector<int> expected = {1, 2, 3, 4, 5, 10, 41, 50, 51, 52};
        CHECK(remaining == expected);
    });

    suite.add("remove fires the callback once on a hit and never on a miss", [] {
        Heap<int> heap;
        for (int value : {4, 8, 2, 6}) heap.push(value);
        hitCalls = 0;
        heap.remove(999, countHit);
        CHECK(hitCalls == 0 && heap.size() == 4);
        heap.remove(8, countHit);
        CHECK(hitCalls == 1 && heap.size() == 3 && !heap.contains(8));
        heap.remove(2, countHit);
        CHECK(hitCalls == 2 && heap.size() == 2);
        CHECK(heap.pop() == 4 && heap.pop() == 6);
    });

    suite.add("remove of the array's last slot and of the root", [] {
        HeapProbe heap;
        for (int value : {4, 2, 7}) heap.push(value);
        heap.remove(7);  // sits at index count-1, nothing to repair
        CHECK(heap.size() == 2 && heap.validHeapArray());
        CHECK(heap.pop() == 2 && heap.pop() == 4);
        heap.push(7);
        heap.push(1);
        heap.remove(1);  // the root
        CHECK(heap.size() == 1 && heap.peek() == 7);
        CHECK(heap.pop() == 7 && heap.empty());
    });

    suite.add("heapify handles empty and single-element ranges", [] {
        int values[3] = {9, 8, 7};
        Heap<int> heap;
        heap.push(1);
        heap.push(2);
        heap.heapify(values, 0);
        CHECK(heap.empty() && heap.size() == 0);
        bool thrown = false;
        try { heap.peek(); } catch (const std::underflow_error&) { thrown = true; }
        CHECK(thrown);
        heap.heapify(values, 1);
        CHECK(heap.size() == 1 && heap.peek() == 9);
        heap.heapify(values, 3);
        CHECK(heap.size() == 3 && heap.peek() == 7);
        CHECK(heap.pop() == 7 && heap.pop() == 8 && heap.pop() == 9);
    });

    suite.add("heapify replaces a bigger heap and grows past capacity", [] {
        Heap<int> heap;
        for (int i = 0; i < 5; ++i) heap.push(i);
        std::vector<int> values;
        for (int i = 200; i > 0; --i) values.push_back(i);
        heap.heapify(values.data(), static_cast<int>(values.size()));
        CHECK(heap.size() == 200);
        for (int i = 1; i <= 200; ++i) CHECK(heap.pop() == i);
        CHECK(heap.empty());
    });

    suite.add("heapify stays linear on reverse-sorted and random input", [] {
        constexpr int N = 4096;
        Heap<int> heap(countingAscending);
        std::vector<int> descending;
        for (int i = N; i >= 1; --i) descending.push_back(i);
        countedCompares = 0;
        heap.heapify(descending.data(), N);
        CHECK(heap.size() == N);
        CHECK(countedCompares < 4 * N);  // a push loop is ~N log N (~40974 here)

        std::vector<int> random;
        unsigned seed = 5u;
        for (int i = 0; i < N; ++i) random.push_back(static_cast<int>(nextRandom(seed) % 100000));
        countedCompares = 0;
        heap.heapify(random.data(), N);
        CHECK(countedCompares < 4 * N);
        std::sort(random.begin(), random.end());
        for (int i = 0; i < N; ++i) CHECK(heap.pop() == random[i]);
    });

    suite.add("push after heapify grows the array correctly", [] {
        int seedValues[8] = {8, 6, 4, 2, 7, 5, 3, 1};
        HeapProbe heap;
        heap.heapify(seedValues, 8);
        CHECK(heap.countOf() == 8 && heap.capacityOf() >= 8);
        for (int i = 100; i < 140; ++i) heap.push(i);
        CHECK(heap.size() == 48);
        CHECK(heap.validHeapArray());
        std::vector<int> expected;
        for (int i = 1; i <= 8; ++i) expected.push_back(i);
        for (int i = 100; i < 140; ++i) expected.push_back(i);
        for (int value : expected) CHECK(heap.pop() == value);
        CHECK(heap.empty());
    });

    suite.add("clear empties the heap and it stays usable", [] {
        Heap<int> heap;
        heap.clear();
        CHECK(heap.empty() && heap.size() == 0);
        for (int value : {3, 1, 2}) heap.push(value);
        heap.clear();
        CHECK(heap.empty() && heap.size() == 0);
        CHECK(!heap.contains(1));
        heap.push(42);
        CHECK(heap.size() == 1 && heap.peek() == 42);
        CHECK(heap.pop() == 42);
    });

    suite.add("copy constructor builds an independent heap", [] {
        Heap<int> original;
        for (int value : {5, 1, 4, 2, 3}) original.push(value);
        Heap<int> copy(original);
        CHECK(copy.size() == 5 && original.size() == 5);
        CHECK(copy.pop() == 1 && copy.pop() == 2);
        CHECK(original.size() == 5 && original.peek() == 1);
        for (int value : {1, 2, 3, 4, 5}) CHECK(original.pop() == value);
        CHECK(copy.pop() == 3 && copy.pop() == 4 && copy.pop() == 5);
        copy.push(9);
        CHECK(copy.size() == 1 && original.empty());
    });

    suite.add("assignment operator copies and survives self-assignment", [] {
        Heap<int> source;
        for (int value : {7, 3, 9}) source.push(value);
        Heap<int> target;
        target.push(100);
        target = source;
        CHECK(target.size() == 3 && source.size() == 3);
        CHECK(target.pop() == 3);
        CHECK(source.size() == 3 && source.peek() == 3);
        target = target;
        CHECK(target.size() == 2 && target.pop() == 7);
        for (int value : {3, 7, 9}) CHECK(source.pop() == value);
        CHECK(source.empty());
    });

    suite.add("iterator visits every element and can remove one", [] {
        Heap<int> heap;
        for (int value = 1; value <= 10; ++value) heap.push(value);
        long long sum = 0;
        int visited = 0;
        for (Heap<int>::Iterator it = heap.begin(); it != heap.end(); ++it) {
            sum += *it;
            ++visited;
        }
        CHECK(visited == 10 && sum == 55);
        Heap<int>::Iterator it = heap.begin();
        const int first = *it;
        it.remove();
        CHECK(heap.size() == 9);
        CHECK(!heap.contains(first));
        CHECK(heap.pop() == 2);
    });

    suite.add("toString formats empty, single and multi element heaps", [] {
        Heap<int> heap;
        CHECK(heap.toString() == "[]");
        heap.push(5);
        CHECK(heap.toString() == "[5]");
        heap.push(3);
        heap.push(2);
        CHECK(heap.toString() == "[2,5,3]");  // raw array order, no trailing comma
        Heap<std::string> words;
        words.push("b");
        words.push("a");
        const std::string text = words.toString([](std::string& s) { return "<" + s + ">"; });
        CHECK(text.size() >= 2 && text.front() == '[' && text.back() == ']');
        CHECK(text.find(",]") == std::string::npos);
        CHECK(text.find("<a>") != std::string::npos && text.find("<b>") != std::string::npos);
    });

    suite.add("pointer heap frees user data exactly once", [] {
        CHECK(Payload::live == 0);
        {
            Heap<Payload*> heap(byPayloadValue, &Heap<Payload*>::free);
            for (int value = 1; value <= 5; ++value) heap.push(new Payload(value));
            CHECK(Payload::live == 5);

            dropCalls = 0;
            Payload* missing = new Payload(999);
            CHECK(Payload::live == 6);
            heap.remove(missing, dropPayload);  // miss: callback must not run
            CHECK(dropCalls == 0 && Payload::live == 6);
            delete missing;
            CHECK(Payload::live == 5);

            heap.remove(heap.peek(), dropPayload);  // hit on the root
            CHECK(dropCalls == 1 && Payload::live == 4 && heap.size() == 4);

            heap.clear();
            CHECK(Payload::live == 0);
            heap.push(new Payload(7));
            CHECK(Payload::live == 1);
        }
        CHECK(Payload::live == 0);  // destructor frees what is left
    });

    suite.add("string heap keeps lexicographic order", [] {
        Heap<std::string> heap;
        for (const char* name : {"cho lon", "ben thanh", "dai hoc bach khoa", "ben xe mien tay"}) {
            heap.push(std::string(name));
        }
        CHECK(heap.peek() == "ben thanh");
        CHECK(heap.pop() == "ben thanh");
        CHECK(heap.pop() == "ben xe mien tay");
        CHECK(heap.pop() == "cho lon");
        CHECK(heap.pop() == "dai hoc bach khoa");
        CHECK(heap.empty());
    });

    return suite.run(argc, argv);
}
