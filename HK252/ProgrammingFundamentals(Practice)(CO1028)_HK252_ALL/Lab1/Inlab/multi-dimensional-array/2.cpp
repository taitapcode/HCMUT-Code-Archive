bool isPrime(int n)
{
  if (n == 2 || n == 3) return 1;
  if (n < 2 || n % 2 == 0 || n % 3 == 0) return 0;
  for (int i = 5; i * i <= n; i += 6)
    if (n % i == 0 || n % (i + 2) == 0) return 0;
  return 1;
}

int primeColumns(int arr[][1000], int row, int col)
{
  int res = 0;
  for (int j = 0; j < col; j++)
  {
    int sum = 0;
    for(int i = 0; i < row; i++) sum += arr[i][j];
    if (isPrime(sum)) res++;
  }
  return res;
}
