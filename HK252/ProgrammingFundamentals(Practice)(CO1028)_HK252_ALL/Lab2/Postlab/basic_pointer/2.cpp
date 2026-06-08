#include <iostream>
using namespace std;

void reverse(int *ptr, int n)
{
  int *left = ptr, *right = ptr + n - 1;
  while(left < right)
  {
    int temp = *left;
    *left = *right;
    *right = temp;

    left++, right--;
  }
}

int main()
{
  int arr[] = {1, 2, 3, 4, 5};
  reverse(arr, sizeof(arr) / sizeof(arr[0]));
  cout << arr[0];
  for (int i = 1; i < sizeof(arr) / sizeof(arr[0]); i++)
  {
    cout << ", " << arr[i];
  }
  return 0;
}
