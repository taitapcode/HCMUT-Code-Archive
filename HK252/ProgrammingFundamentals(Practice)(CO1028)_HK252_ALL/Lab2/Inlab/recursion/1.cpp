#include <bits/stdc++.h>
using namespace std;

int recursiveSum(int arr[], int size)
{
  if (size == 1) return arr[0];
  return arr[0] + recursiveSum(arr + 1, size - 1);
}

int main()
{
  int arr[] = {1,2,3,4,5};
  int size = 5;
  cout << recursiveSum(arr, size);

  return 0;
}
