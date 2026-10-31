#include <algorithm>
#include <cassert>
#include <chrono>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include "QuickSort.h"
#include "tests/TestUtils.h"

// Custom Struct
struct Student {
    int id;
    int score;
    std::string name;

    bool operator<(const Student& other) const {
        return score < other.score;
    }
    bool operator>(const Student& other) const {
        return score > other.score;
    }
};

// Custom Pivot Functions
static int pivotFirst(int*, int) {
    return 0;
}

static int pivotLast(int*, int size) {
    return size - 1;
}

static int pivotMid(int*, int size) {
    return size / 2;
}

static int pivotMedianOf3(int* arr, int size) {
    if (size < 3) return 0;
    int mid = size / 2;
    int a = arr[0], b = arr[mid], c = arr[size - 1];
    if ((a <= b && b <= c) || (c <= b && b <= a)) return mid;
    if ((b <= a && a <= c) || (c <= a && a <= b)) return 0;
    return size - 1;
}

int main(int argc, char* argv[]) {
    PublicTestSuite suite("Q4 - QuickSort (Comprehensive & Stress Tests)");

    suite.add("Boundary & Nullptr safety", []() {
        QuickSort<int> sorter;
        sorter.sort(nullptr, 0);
        sorter.sort(nullptr, 5);

        int single = 99;
        sorter.sort(&single, 1);
        requireEqual(single, 99, "size 1 unchanged");

        int twoRev[] = {10, 2};
        sorter.sort(twoRev, 2);
        requireEqual(twoRev[0], 2, "size 2 sorted [0]");
        requireEqual(twoRev[1], 10, "size 2 sorted [1]");

        int twoSorted[] = {2, 10};
        sorter.sort(twoSorted, 2);
        requireEqual(twoSorted[0], 2, "size 2 already sorted [0]");
        requireEqual(twoSorted[1], 10, "size 2 already sorted [1]");

        int twoEqual[] = {5, 5};
        sorter.sort(twoEqual, 2);
        requireEqual(twoEqual[0], 5, "size 2 equal [0]");
        requireEqual(twoEqual[1], 5, "size 2 equal [1]");
    });

    suite.add("All Pivot Selection Strategies (First, Mid, Last, Median3)", []() {
        std::vector<int (*)(int*, int)> pivots = {pivotFirst, pivotMid, pivotLast, pivotMedianOf3};
        std::vector<std::string> names = {"pivotFirst", "pivotMid", "pivotLast", "pivotMedianOf3"};

        for (size_t p = 0; p < pivots.size(); ++p) {
            QuickSort<int> sorter(pivots[p]);

            // Test on sizes 2 to 30 with sorted, reverse, and all-equal
            for (int sz = 2; sz <= 30; ++sz) {
                std::vector<int> sortedArr(sz), revArr(sz), equalArr(sz, 42);
                for (int i = 0; i < sz; ++i) {
                    sortedArr[i] = i;
                    revArr[i] = sz - i;
                }

                sorter.sort(sortedArr.data(), sz);
                for (int i = 0; i < sz; ++i) {
                    requireEqual(sortedArr[i], i, names[p] + " sorted array");
                }

                sorter.sort(revArr.data(), sz);
                for (int i = 0; i < sz; ++i) {
                    requireEqual(revArr[i], i + 1, names[p] + " reverse array");
                }

                sorter.sort(equalArr.data(), sz);
                for (int i = 0; i < sz; ++i) {
                    requireEqual(equalArr[i], 42, names[p] + " equal array");
                }
            }
        }
    });

    suite.add("Tricky array patterns (Plateaus, Alternating, Extreme values)", []() {
        QuickSort<int> sorter;

        // Alternating elements [0, 1, 0, 1, ...]
        std::vector<int> alt(50);
        for (size_t i = 0; i < alt.size(); ++i) alt[i] = (i % 2 == 0) ? 0 : 1;
        sorter.sort(alt.data(), alt.size());
        for (size_t i = 0; i < 25; ++i) requireEqual(alt[i], 0, "alternating 0s");
        for (size_t i = 25; i < 50; ++i) requireEqual(alt[i], 1, "alternating 1s");

        // Few unique elements (plateaus)
        std::vector<int> plateaus = {3, 3, 3, 1, 1, 2, 2, 2, 2, 1, 3, 2};
        sorter.sort(plateaus.data(), plateaus.size());
        for (size_t i = 1; i < plateaus.size(); ++i) {
            requireTrue(plateaus[i - 1] <= plateaus[i], "plateaus sorted");
        }

        // Extreme values (INT_MIN, INT_MAX)
        int extreme[] = {2147483647, -2147483647 - 1, 0, -1, 1};
        sorter.sort(extreme, 5);
        requireEqual(extreme[0], -2147483647 - 1, "extreme min");
        requireEqual(extreme[1], -1, "extreme -1");
        requireEqual(extreme[2], 0, "extreme 0");
        requireEqual(extreme[3], 1, "extreme 1");
        requireEqual(extreme[4], 2147483647, "extreme max");
    });

    suite.add("Custom Types (Students with multi-field comparator)", []() {
        // Sort descending by score, tie-break by name ascending
        auto cmpStudent = [](Student& a, Student& b) -> int {
            if (a.score > b.score) return -1;
            if (a.score < b.score) return 1;
            if (a.name < b.name) return -1;
            if (a.name > b.name) return 1;
            return 0;
        };

        Student students[] = {
            {1, 85, "Bob"},
            {2, 90, "Alice"},
            {3, 85, "Charlie"},
            {4, 70, "David"},
            {5, 90, "Anna"}
        };

        QuickSort<Student> sorter;
        sorter.sort(students, 5, cmpStudent);

        // Expected order:
        // Alice (90), Anna (90), Bob (85), Charlie (85), David (70)
        requireEqual(students[0].name, std::string("Alice"), "Student 1");
        requireEqual(students[1].name, std::string("Anna"), "Student 2");
        requireEqual(students[2].name, std::string("Bob"), "Student 3");
        requireEqual(students[3].name, std::string("Charlie"), "Student 4");
        requireEqual(students[4].name, std::string("David"), "Student 5");
    });

    suite.add("Random Fuzzing (1,000 random arrays vs std::sort)", []() {
        std::mt19937 rng(42);
        std::vector<int (*)(int*, int)> pivots = {nullptr, pivotFirst, pivotMid, pivotLast};

        for (auto pivotFn : pivots) {
            QuickSort<int> sorter(pivotFn);
            for (int test = 0; test < 250; ++test) {
                int size = rng() % 80;
                std::vector<int> actual(size);
                for (int& x : actual) x = (rng() % 100) - 50;

                std::vector<int> expected = actual;
                std::sort(expected.begin(), expected.end());

                sorter.sort(actual.data(), size);
                requireTrue(actual == expected, "Fuzzing matches std::sort");
            }
        }
    });

    suite.add("Large Array Scale & Performance (50,000 elements)", []() {
        const int N = 50000;
        std::vector<int> arr(N);
        std::mt19937 rng(999);
        for (int& x : arr) x = rng();

        QuickSort<int> sorter;
        auto start = std::chrono::high_resolution_clock::now();
        sorter.sort(arr.data(), N);
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

        for (int i = 1; i < N; ++i) {
            requireTrue(arr[i - 1] <= arr[i], "large array sorted order");
        }
        std::cout << "(50k elements sorted in " << elapsedMs << " ms) ";
        requireTrue(elapsedMs < 2000, "50k elements finished within 2 seconds");
    });

    return suite.run(argc, argv);
}
