#ifndef QUICKSORT_H
#define QUICKSORT_H

#include <stdexcept>

#include "ISort.h"
using namespace std;

template <class T>
class QuickSort : public ISort<T>
{
private:
  int (*pivotSelection)(T*, int);

public:
  QuickSort(int (*pivotSelection)(T*, int) = 0)
      : pivotSelection(pivotSelection) {}

  void sort(T array[], int size, int (*comparator)(T&, T&) = 0) override
  {
    if (array == nullptr || size <= 1) return;
    quickSort(array, 0, size - 1, comparator);
  }

private:
  int compare(T& a, T& b, int (*comparator)(T&, T&))
  {
    if (comparator) return comparator(a, b);
    return SortSimpleOrder<T>::compare4Ascending(a, b);
  }

  void quickSort(T array[], int left, int right,
                 int (*comparator)(T&, T&) = 0)
  {
    if (left >= right) return;
    int partitionIndex = partition(array, left, right, comparator);
    quickSort(array, left, partitionIndex, comparator);
    quickSort(array, partitionIndex + 1, right, comparator);
  }

  int partition(T array[], int left, int right,
                int (*comparator)(T&, T&) = 0)
  {
    int pivotIdx = left + (right - left) / 2;
    if (pivotSelection) pivotIdx = left + pivotSelection(&array[left], right - left + 1);
    std::swap(array[left], array[pivotIdx]);
    T pivotValue = array[left];

    int i = left - 1, j = right + 1;
    while (true)
    {
      do
      {
        i++;
      } while (compare(array[i], pivotValue, comparator) < 0);

      do
      {
        j--;
      } while (compare(array[j], pivotValue, comparator) > 0);

      if (i >= j) return j;
      std::swap(array[i], array[j]);
    }
  }
};

#endif
