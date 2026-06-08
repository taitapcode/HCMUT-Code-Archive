bool completeNum(int N)
{
  if (N < 2) return false;

  int sum = 1;
  for(int i = 2; i * i <= N; i++)
  {
    if (N % i == 0)
    {
      sum += i;
      if (i * i != N) sum += N / i;
    }
  }

  return sum == N;
}
