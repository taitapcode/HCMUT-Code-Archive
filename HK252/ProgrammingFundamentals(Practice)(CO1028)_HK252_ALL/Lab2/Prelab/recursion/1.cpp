int calculate_power(int n, int e)
{
  if (e < 1) return 1;
  return n * calculate_power(n, e - 1);
}
