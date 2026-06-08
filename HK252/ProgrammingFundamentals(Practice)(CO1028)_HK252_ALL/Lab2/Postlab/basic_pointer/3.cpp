#include <iostream>
using namespace std;

bool isSymmetry(int *head, int *tail)
{
  while(head < tail)
  {
    if (*head != *tail) return 0;
    head++, tail--;
  }

  return 1;
}

int main()
{
  int arr[] = {1, 2, 1};
  cout << isSymmetry(arr, arr + (sizeof(arr) / sizeof(arr[0])) - 1);

  return 0;
}
