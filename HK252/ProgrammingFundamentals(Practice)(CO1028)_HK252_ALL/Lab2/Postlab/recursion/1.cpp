#include <bits/stdc++.h>
#define el '\n'
#define ll long long
#define db double
#define ldb long double
using namespace std;

int findOccurrences(int nums[], int size, int element)
{
  if (!size) return 0;
  return (nums[size - 1] == element) + findOccurrences(nums, size - 1, element);
}

int main()
{
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  int nums[] = {1,2,3};
  cout << findOccurrences(nums, 3, 3);

  return 0;
}
