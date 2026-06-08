int* zeros(int n)
{
  if (n < 1) return nullptr;
  return new int[n]();
}

void shallowCopy(int*& newArr, int*& arr)
{
  newArr = arr;
}

int** deepCopy(int** matrix, int r, int c)
{
  if (r < 1 || c < 1 || matrix == nullptr) return nullptr;

  int **copy = new int*[r];
  copy[0] = new int[r * c];
  for(int i = 1; i < r; i++) copy[i] = copy[0] + i * c;

  for(int i = 0; i < r; i++)
    for(int j = 0; j < c; j++)
      copy[i][j] = matrix[i][j];

  return copy;
}

void deleteMatrix(int**& matrix, int r)
{
  for(int i = 0; i < r; i++) delete[] matrix[i];
  delete[] matrix;
  matrix = nullptr;
}

void insertRow(int**& matrix, int r, int c, int* rowArr, int row)
{
  int **newMatrix = new int*[r + 1];
  newMatrix[0] = new int[(r + 1) * c];
  for(int i = 1; i <= r; i++) newMatrix[i] = newMatrix[0] + c * i;

  for(int i = 0; i <= r; i++)
    for(int j = 0; j < c; j++)
    {
      if (i < row) newMatrix[i][j] = matrix[i][j];
      else if (i == row) newMatrix[i][j] = rowArr[j];
      else newMatrix[i][j] = matrix[i - 1][j];
    }


  for(int i = 0; i < r; i++) delete[] matrix[i];
  delete[] matrix;
  matrix = newMatrix;
}
