#include <iostream>
using namespace std;

int findMax(int *ptr, int n)
{
  int mx = *ptr;
  for(int *p = ptr; p != ptr + n; p++)
    if (mx < *p) mx = *p;

  return mx;
}

int main()
{
  int arr[] = {1, 2, 3, 4, 5};
  cout << findMax(arr, sizeof(arr) / sizeof(arr[0]));

  return 0;
}
