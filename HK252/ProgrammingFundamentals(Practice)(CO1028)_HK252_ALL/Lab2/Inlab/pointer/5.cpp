int** transposeMatrix(int** matrix, int r, int c)
{
  if (!r || !c) return nullptr;
  int **newMatrix = new int*[c];
  for(int i = 0; i < c; i++)
  {
    newMatrix[i] = new int[r];
    for(int j = 0; j < r; j++)
      newMatrix[i][j] = matrix[j][i];
  }

  return newMatrix;
}
