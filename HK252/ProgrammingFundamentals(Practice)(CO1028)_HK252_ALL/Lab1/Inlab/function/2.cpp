bool isPrime(int n)
{
  if (n < 2) return 0;
  if (n < 4) return 1;
  if (n % 2 == 0 || n % 3 == 0) return 0;
  for (int i = 5; i * i <= n; i += 6)
    if (n % i == 0 || n % (i + 2) == 0) return 0;
  return 1;
}

bool isSpecialNumber(int n)
{
  bool check = isPrime(n);
  int sumDigit = 0;
  while (n > 0) {
    sumDigit += n % 10;
    n /= 10;
  }

  return check && isPrime(sumDigit);
}
