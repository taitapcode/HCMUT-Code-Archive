#include "tests/TestUtils.h"
#include "QuickSort.h"
#include <string>
#include <vector>
#include <algorithm>
#include <random>

using namespace std;

// Custom types
struct StudentQ4 {
    int id;
    string name;
    double gpa;
    
    bool operator==(const StudentQ4& other) const {
        return id == other.id && name == other.name && gpa == other.gpa;
    }
    bool operator<(const StudentQ4& other) const {
        return id < other.id;
    }
    bool operator>(const StudentQ4& other) const {
        return id > other.id;
    }
};

// Comparators
int compareStudentMultiQ4(StudentQ4& a, StudentQ4& b) {
    // Primary: GPA ascending
    if (a.gpa < b.gpa) return -1;
    if (a.gpa > b.gpa) return 1;
    // Secondary: ID descending
    if (a.id > b.id) return -1;
    if (a.id < b.id) return 1;
    return 0;
}

int compareOddEvenQ4(int& a, int& b) {
    // Evens before odds
    bool aEven = (a % 2 == 0);
    bool bEven = (b % 2 == 0);
    if (aEven && !bEven) return -1;
    if (!aEven && bEven) return 1;
    // Then ascending
    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
}

int compareNeverZeroQ4(int& a, int& b) {
    if (a <= b) return -1;
    return 1;
}

// Pivot Selectors
int pivotLastQ4(int*, int size) {
    return size - 1; // Always last element
}

void registerDeepQ4Tests(int argc, char* argv[]) {
    PublicTestSuite suite("Q4_QuickSort_Deep");

    suite.add("Negative size handling", []() {
        QuickSort<int> qs;
        int arr[] = {1, 2, 3};
        // Should not crash, should just return
        qs.sort(arr, -1);
        requireEqual(arr[0], 1, "Element 0 intact");
        requireEqual(arr[1], 2, "Element 1 intact");
        requireEqual(arr[2], 3, "Element 2 intact");
        
        qs.sort(arr, 0);
        requireEqual(arr[0], 1, "Element 0 intact");
    });

    suite.add("Size 2 all combinations", []() {
        QuickSort<int> qs;
        
        int arr1[] = {1, 2};
        qs.sort(arr1, 2);
        requireEqual(arr1[0], 1, "[1, 2] -> 1");
        requireEqual(arr1[1], 2, "[1, 2] -> 2");

        int arr2[] = {2, 1};
        qs.sort(arr2, 2);
        requireEqual(arr2[0], 1, "[2, 1] -> 1");
        requireEqual(arr2[1], 2, "[2, 1] -> 2");

        int arr3[] = {2, 2};
        qs.sort(arr3, 2);
        requireEqual(arr3[0], 2, "[2, 2] -> 2");
        requireEqual(arr3[1], 2, "[2, 2] -> 2");
    });

    suite.add("Size 3 all permutations", []() {
        QuickSort<int> qs;
        int perms[6][3] = {
            {1, 2, 3}, {1, 3, 2}, {2, 1, 3},
            {2, 3, 1}, {3, 1, 2}, {3, 2, 1}
        };
        for (int i = 0; i < 6; i++) {
            qs.sort(perms[i], 3);
            requireEqual(perms[i][0], 1, "Pos 0 is 1");
            requireEqual(perms[i][1], 2, "Pos 1 is 2");
            requireEqual(perms[i][2], 3, "Pos 2 is 3");
        }
    });

    suite.add("Instability demonstration", []() {
        struct Item {
            int val;
            int orig_idx;
            bool operator<(const Item& other) const { return val < other.val; }
            bool operator>(const Item& other) const { return val > other.val; }
        };
        Item arr[] = { {2, 0}, {1, 1}, {2, 2}, {1, 3} };
        
        QuickSort<Item> qs;
        qs.sort(arr, 4, [](Item& a, Item& b) {
            if (a.val < b.val) return -1;
            if (a.val > b.val) return 1;
            return 0;
        });
        
        requireEqual(arr[0].val, 1, "Value is 1");
        requireEqual(arr[1].val, 1, "Value is 1");
        requireEqual(arr[2].val, 2, "Value is 2");
        requireEqual(arr[3].val, 2, "Value is 2");
        
        // Quicksort Hoare is typically not stable, so the original indices might not be perfectly ordered
        // But we just verify it sorts correctly.
    });

    suite.add("Strings array", []() {
        QuickSort<string> qs;
        string arr[] = {"banana", "apple", "cherry", "date", "apple"};
        qs.sort(arr, 5);
        requireEqual(arr[0], string("apple"), "0");
        requireEqual(arr[1], string("apple"), "1");
        requireEqual(arr[2], string("banana"), "2");
        requireEqual(arr[3], string("cherry"), "3");
        requireEqual(arr[4], string("date"), "4");
    });

    suite.add("Double/float array", []() {
        QuickSort<double> qs;
        double arr[] = {3.14, -1.5, 2.71, 0.0, 3.14};
        qs.sort(arr, 5);
        requireEqual(arr[0], -1.5, "0");
        requireEqual(arr[1], 0.0, "1");
        requireEqual(arr[2], 2.71, "2");
        requireEqual(arr[3], 3.14, "3");
        requireEqual(arr[4], 3.14, "4");
    });

    suite.add("Odd/Even comparator", []() {
        QuickSort<int> qs;
        int arr[] = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
        qs.sort(arr, 11, compareOddEvenQ4);
        
        // Evens: 4, 2, 6 -> 2, 4, 6
        // Odds: 3, 1, 1, 5, 9, 5, 3, 5 -> 1, 1, 3, 3, 5, 5, 5, 9
        int expected[] = {2, 4, 6, 1, 1, 3, 3, 5, 5, 5, 9};
        for (int i = 0; i < 11; i++) {
            requireEqual(arr[i], expected[i], "Correct odd/even ordering");
        }
    });

    suite.add("Pivot selector last element", []() {
        QuickSort<int> qs(pivotLastQ4);
        int arr[] = {3, 1, 4, 1, 5, 9};
        qs.sort(arr, 6);
        requireEqual(arr[0], 1, "0");
        requireEqual(arr[5], 9, "5");
    });

    suite.add("Pipe-organ array", []() {
        QuickSort<int> qs;
        const int N = 1000;
        int arr[N];
        for (int i = 0; i < N / 2; i++) arr[i] = i;
        for (int i = N / 2; i < N; i++) arr[i] = N - i;
        
        qs.sort(arr, N);
        
        for (int i = 0; i < N - 1; i++) {
            requireTrue(arr[i] <= arr[i + 1], "Sorted pipe-organ");
        }
    });

    suite.add("Sawtooth arrays", []() {
        QuickSort<int> qs;
        const int N = 1000;
        int arr[N];
        for (int i = 0; i < N; i++) {
            arr[i] = i % 10;
        }
        qs.sort(arr, N);
        
        for (int i = 0; i < N - 1; i++) {
            requireTrue(arr[i] <= arr[i + 1], "Sorted sawtooth");
        }
    });

    suite.add("All same except one outlier", []() {
        QuickSort<int> qs;
        int arr[] = {5, 5, 5, 5, 1, 5, 5, 5};
        qs.sort(arr, 8);
        requireEqual(arr[0], 1, "Outlier at start");
        for (int i = 1; i < 8; i++) {
            requireEqual(arr[i], 5, "Rest are 5");
        }
    });

    suite.add("Sorted then one swap", []() {
        QuickSort<int> qs;
        const int N = 100;
        int arr[N];
        for (int i = 0; i < N; i++) arr[i] = i;
        
        swap(arr[10], arr[90]);
        
        qs.sort(arr, N);
        for (int i = 0; i < N; i++) {
            requireEqual(arr[i], i, "Restored sorted order");
        }
    });

    suite.add("Negative numbers only", []() {
        QuickSort<int> qs;
        int arr[] = {-10, -1, -5, -100, -20, -3};
        qs.sort(arr, 6);
        requireEqual(arr[0], -100, "1");
        requireEqual(arr[1], -20, "2");
        requireEqual(arr[2], -10, "3");
        requireEqual(arr[3], -5, "4");
        requireEqual(arr[4], -3, "5");
        requireEqual(arr[5], -1, "6");
    });

    suite.add("Sort struct by multiple fields", []() {
        QuickSort<StudentQ4> qs;
        StudentQ4 arr[] = {
            {2, "B", 3.5},
            {1, "A", 3.5},
            {3, "C", 4.0},
            {4, "D", 2.0}
        };
        
        qs.sort(arr, 4, compareStudentMultiQ4);
        
        // Expected order:
        // GPA ascending: 2.0, 3.5, 3.5, 4.0
        // For GPA 3.5, ID descending: ID 2, then ID 1
        
        requireEqual(arr[0].id, 4, "0"); // GPA 2.0
        requireEqual(arr[1].id, 2, "1"); // GPA 3.5, ID 2
        requireEqual(arr[2].id, 1, "2"); // GPA 3.5, ID 1
        requireEqual(arr[3].id, 3, "3"); // GPA 4.0
    });

    suite.run(argc, argv);
}

int main(int argc, char* argv[]) {
    registerDeepQ4Tests(argc, argv);
    return 0;
}
