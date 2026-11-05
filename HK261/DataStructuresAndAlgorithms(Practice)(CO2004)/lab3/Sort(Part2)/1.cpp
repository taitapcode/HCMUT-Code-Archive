#include <iostream>
using namespace std;

template <typename T>
static T* Partition(T* start, T* end)
{
  T* last = end - 1;
  T p = *start;
  T* i = start;
  T* j = end;
  do
  {
    do
    {
      ++i;
    } while (i <= last && *i < p);
    do
    {
      --j;
    } while (*j > p);
    if (i < j) swap(*i, *j);
  } while (i < j);
  swap(*start, *j);
  return j;
}

template <typename T>
static void QuickSort(T* start, T* end)
{
  if (start >= end) return;
  T* pivot = Partition(start, end);
  cout << (pivot - start) << " ";
  QuickSort(start, pivot);
  QuickSort(pivot + 1, end);
}
