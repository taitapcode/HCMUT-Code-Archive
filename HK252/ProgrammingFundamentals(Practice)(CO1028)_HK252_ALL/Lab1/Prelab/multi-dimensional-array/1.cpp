#include <climits>

int findMaxColumn(int arr[][1000], int row, int col)
{
  int maxCol = INT_MIN, colIndex = -1;
  for (int j = 0; j < col; j++)
  {
    int colSum = 0;
    for (int i = 0; i < row; i++) colSum += arr[i][j];
    if (colSum > maxCol)
    {
      maxCol = colSum;
      colIndex = j;
    }
  }
  return colIndex;
}
