int findGCD(int a, int b)
{
  if (b) return findGCD(b, a % b);
  return a;
}

int findLCM(int a, int b)
{
  return (a * b) / findGCD(a, b);
}
