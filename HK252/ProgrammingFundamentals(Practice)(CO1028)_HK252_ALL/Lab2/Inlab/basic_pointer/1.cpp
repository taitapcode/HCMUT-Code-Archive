#include <bits/stdc++.h>
#define el '\n'
#define ll long long
#define db double
#define ldb long double
using namespace std;

int calcSum(int *ptr, int n)
{
  int sum = 0;
  for(int i = 0; i < n; i++) sum += *(ptr + i);
  return sum;
}

int main()
{
  int arr[] = {1, 2, 3, 4, 5};
  cout << calcSum(arr, sizeof(arr) / sizeof(arr[0]));

  // int arr[] = {0, -1, 5, 6, -5, 1, -9, -10, -6, 3};
  // cout << calcSum(arr, sizeof(arr) / sizeof(arr[0]));
  return 0;
}
