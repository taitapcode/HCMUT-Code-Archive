#include <deque>
#include <vector>

using namespace std;

int sumOfMaxSubarray(vector<int>& nums, int k)
{
  int sum = 0, n = nums.size();
  deque<int> dq;

  for (int i = 0; i < n; i++)
  {
    while (!dq.empty() && dq.front() <= i - k) dq.pop_front();
    while (!dq.empty() && nums[dq.back()] < nums[i]) dq.pop_back();
    dq.push_back(i);

    if (i >= k - 1) sum += nums[dq.front()];
  }

  return sum;
}
