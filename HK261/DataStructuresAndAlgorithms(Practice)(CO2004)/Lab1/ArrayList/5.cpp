#include <vector>

bool consecutiveOnes(std::vector<int>& nums)
{
  bool checked = false, check = false;

  for (int& x : nums)
    if (x == 1)
    {
      if (checked && !check) return false;
      checked = check = true;
    }
    else
      check = false;

  return true;
}
