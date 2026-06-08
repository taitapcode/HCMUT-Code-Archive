int* flatten(int** matrix, int r, int c)
{
  int *newMatrix = new int[r * c];
  for(int i = 0; i < r; i++)
    for(int j = 0; j < c; j++)
      newMatrix[i * c + j] = matrix[i][j];
  return newMatrix;
}
