#include <vector>
using namespace std;

vector<int> updateArrayPerRange(vector<int>& nums, vector<vector<int>>& operations)
{
  vector<int> sum(nums.size() + 1, 0);
  for (const vector<int>& operation : operations)
  {
    sum[operation[0]] += operation[2];
    sum[operation[1] + 1] -= operation[2];
  }

  for (std::size_t i = 0; i < nums.size(); i++)
  {
    if (i > 0) sum[i] += sum[i - 1];
    nums[i] += sum[i];
  }
  return nums;
}
