int ascendingRows(int arr[][1000], int row, int col)
{
  int res = 0;
  for (int i = 0; i < row; i++)
  {
    bool check = 1;
    for (int j = 1; j < col; j++)
      if (arr[i][j] <= arr[i][j - 1])
      {
        check = 0;
        break;
      }
    if (check) res++;
  }

  return res;
}
