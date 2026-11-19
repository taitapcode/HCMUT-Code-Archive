#include <vector>

// int equalSumIndex(std::vector<int>& nums)
// {
//   std::size_t n = nums.size();
//   std::vector<int> sum(nums.size() + 1, 0);
//   sum[1] = nums[0];
//   for (std::size_t i = 2; i <= n; i++) sum[i] = sum[i - 1] + nums[i - 1];
//
//   for (std::size_t i = 0; i < n; i++)
//     if (sum[i] - sum[0] == sum[n] - sum[i + 1]) return i;
//
//   return -1;
// }

int equalSumIndex(std::vector<int>& nums)
{
  std::size_t n = nums.size();
  int sum = 0, curr = 0;
  for (int& x : nums) sum += x;

  for (int i = 0; i < n; i++)
  {
    if (2 * curr + nums[i] == sum) return i;
    curr += nums[i];
  }

  return -1;
}
