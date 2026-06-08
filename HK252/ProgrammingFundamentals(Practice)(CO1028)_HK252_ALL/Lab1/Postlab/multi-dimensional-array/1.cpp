#include <iostream>
#include <vector>
#define el '\n'
using namespace std;

bool isPrime(int n)
{
  if (n < 2) return 0;
  if (n < 4) return 1;
  if (n % 2 == 0 || n % 3 == 0) return 0;
  for (int i = 5; i * i <= n; i += 6)
    if (n % i == 0 || n % (i + 2) == 0) return 0;
  return 1;
}

int specialCells(int arr[][1000], int row, int col)
{
  int res = 0;
  vector<int> sumRow(row, 0), sumCol(col, 0);

  for(int i = 0; i < row; i++)
    for(int j = 0; j < col; j++)
    {
      sumRow[i] += arr[i][j];
      sumCol[j] += arr[i][j];
    }

  for(int i = 0; i < row; i++)
    for(int j = 0; j < col; j++)
      if (isPrime(sumRow[i]) && isPrime(sumCol[j])) res++;

  return res;
}

int main()
{
  cout << "TEST1" << el;
  int arr[][1000] = {{37,53,74},{12,37,60},{98,13,15}};
  cout << specialCells(arr,3,3) << el;

  cout << "TEST2" << el;
  int arr2[][1000] = {{1,87,26},{97,95,88},{57,60,46}};;
  cout << specialCells(arr2,3,3) << el;

  return 0;
}
