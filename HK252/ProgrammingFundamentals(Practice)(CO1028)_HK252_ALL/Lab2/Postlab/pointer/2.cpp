bool deleteRow(int**& matrix, int r, int c, int row)
{
  if (!r || !c) return 0;
  if (row < 0 || row >= r) return 0;

  if (r == 1)
  {
    delete[] matrix[0];
    delete matrix;
    matrix = nullptr;

    return 1;
  }

  int **newMatrix = new int*[r - 1];
  for(int i = 0; i < row; i++) newMatrix[i] = matrix[i];
  for(int i = row + 1; i < r; i++) newMatrix[i - 1] = matrix[i];

  delete[] matrix[row];
  delete[] matrix;

  matrix = newMatrix;

  return 1;
}
