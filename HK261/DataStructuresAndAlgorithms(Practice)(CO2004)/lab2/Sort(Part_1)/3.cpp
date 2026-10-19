#include <iostream>
using namespace std;

template <class T>
class Sorting
{
private:
  static void printArray(T* start, T* end)
  {
    int size = end - start;
    for (int i = 0; i < size; i++)
      cout << start[i] << " ";
    cout << endl;
  }

public:
  static void sortSegment(T* start, T* end, int segment_idx, int cur_segment_total)
  {
    int size = end - start;
    for (int i = segment_idx + cur_segment_total; i < size; i += cur_segment_total)
    {
      T key = start[i];
      int j = i - cur_segment_total;
      while (j >= 0 && start[j] > key)
      {
        start[j + cur_segment_total] = start[j];
        j -= cur_segment_total;
      }
      start[j + cur_segment_total] = key;
    }
  }

  static void ShellSort(T* start, T* end, int* num_segment_list, int num_phases)
  {
    for (int phase = num_phases - 1; phase >= 0; phase--)
    {
      int segments = num_segment_list[phase];
      for (int idx = 0; idx < segments; idx++)
      {
        sortSegment(start, end, idx, segments);
      }
      cout << segments << " segments: ";
      printArray(start, end);
    }
  }
};
