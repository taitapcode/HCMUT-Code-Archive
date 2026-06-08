int** insertCol(int**& matrix, int r, int c, int* colArr, int col)
{
  for(int i = 0; i < r; i++)
  {
    int *newRow = new int[c + 1];

    for(int j = 0; j <= c; j++)
    {
      if (j < col) newRow[j] = matrix[i][j];
      else if (j == col) newRow[j] = colArr[i];
      else newRow[j] = matrix[i][j - 1];
    }

    delete[] matrix[i];
    matrix[i] = newRow;
  }

  return matrix;
}
