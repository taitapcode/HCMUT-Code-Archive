#include <iostream>
using namespace std;

template <class T>
class Sorting
{
public:
  static void printArray(T* start, T* end)
  {
    int size = end - start;
    for (int i = 0; i < size - 1; i++)
      cout << start[i] << ", ";
    cout << start[size - 1];
    cout << endl;
  }

  static void selectionSort(T* start, T* end);
};

template <class T>
void Sorting<T>::selectionSort(T* start, T* end)
{
  int size = end - start;
  for (int i = 0; i < size - 1; i++)
  {
    int minIdx = i;
    for (int j = i + 1; j < size; j++)
    {
      if (start[j] < start[minIdx])
        minIdx = j;
    }

    if (minIdx != i)
    {
      T temp = start[i];
      start[i] = start[minIdx];
      start[minIdx] = temp;
    }

    printArray(start, end);
  }
}
