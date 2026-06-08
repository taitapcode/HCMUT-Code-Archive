bool deleteCol(int**& matrix, int r, int c, int col)
{
  if (!r || !c) return 0;
  if (col < 0 || col >= c) return 0;

  if (c == 1)
  {
    for(int i = 0; i < r; i++) delete[] matrix[i];
    delete[] matrix;

    matrix = nullptr;
    return 1;
  }

  for(int i = 0; i < r; i++)
  {
    int *newRow = new int[c - 1];
    for(int j = 0; j < col; j++) newRow[j] = matrix[i][j];
    for(int j = col + 1; j < c; j++) newRow[j - 1] = matrix[i][j];

    delete[] matrix[i];
    matrix[i] = newRow;
  }

  return 1;
}
