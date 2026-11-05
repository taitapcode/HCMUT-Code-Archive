#include <vector>
using namespace std;

namespace Sorting
{
void printArray(int* start, int* end)
{
}
}  // namespace Sorting

template <typename T>
static void merge(T* left, T* middle, T* right)
{
  vector<T> tmp;

  T* i = left;
  T* j = middle + 1;

  while (i <= middle && j <= right)
  {
    if (!(*j < *i))
      tmp.push_back(*(i++));
    else
      tmp.push_back(*(j++));
  }

  while (i <= middle) tmp.push_back(*(i++));
  while (j <= right) tmp.push_back(*(j++));

  for (int k = 0; k < (int)tmp.size(); k++) left[k] = tmp[k];
  Sorting::printArray(left, right);
}

template <typename T>
static void mergeSort(T* start, T* end)
{
  if (start >= end) return;

  T* middle = start + (end - start) / 2;

  mergeSort(start, middle);
  mergeSort(middle + 1, end);
  merge(start, middle, end);
}
