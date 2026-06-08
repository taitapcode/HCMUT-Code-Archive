#include <math.h>

int diagonalDiff(int arr[][1000], int row, int col, int x, int y)
{
  int d1 = 0, d2 = 0;
  for(int i = 0; i < row; i++)
    for(int j = 0; j < col; j++)
    {
      if (i - j == x - y) d1 += arr[i][j];
      if (i + j == x + y) d2 += arr[i][j];
    }
  return abs(d1 - d2);
}
