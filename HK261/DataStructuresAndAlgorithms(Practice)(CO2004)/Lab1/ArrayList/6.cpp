#include <algorithm>

int buyCar(int* nums, int length, int k)
{
  std::sort(nums, nums + length);

  int count = 0;
  for (int i = 0; i < length; i++)
  {
    k -= nums[i];
    if (k < 0) break;
    ++count;
  }

  return count;
}
