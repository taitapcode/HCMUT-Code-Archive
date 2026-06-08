int solve(int target, long long x)
{
  if(!target) return 1;
  if (x * x > target) return 0;

  int res = solve(target - x * x, x + 1);
  res += solve(target, x + 1);

  return res;
}

int countWaySumOfSquare(int x)
{
  if (x <= 0) return 0;
  return solve(x, 1);
}
