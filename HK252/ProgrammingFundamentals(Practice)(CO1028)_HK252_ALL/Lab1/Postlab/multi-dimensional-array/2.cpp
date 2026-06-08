#include <iostream>
#include <vector>
#include <string>
#define el '\n'
using namespace std;

int subMatrix(int arr[][1000], int row, int col)
{
  int ans = 0;
  for(int i = 1; i < row; i++)
    for(int j = 1; j < col; j++)
      ans += (arr[i][j] + arr[i - 1][j] + arr[i][j - 1] + arr[i - 1][j - 1]) & 1;
  return ans;
}

int main()
{
  cout << "TEST1" << el;
  int arr[][1000] = {{66,16,71},{25,81,61},{2,10,34}};
  cout << subMatrix(arr,3, 3) << el;

  cout << "TEST2" << el;
  int arr2[][1000] ={{44,45,89},{82,91,34},{83,87,33},{65,51,66}};
  cout << subMatrix(arr2,4, 3);

  return 0;
}
