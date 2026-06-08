#include<bits/stdc++.h>
#define el '\n'
#define ll long long
#define db double
#define ldb long double
#define FOR(i, a, b) for(int i = a; i < b; i++)
#define FO(i, n) for(int i = 0; i < n; i++)
#define FE(x, a) for (auto &x : a)
#define sp(n) setprecision(n)
using namespace std;

bool checkPrimeNumber(int n)
{
  if (n < 2) return 0;
  if (n == 2 || n == 3 || n == 5) return 1;
  if (n % 2 == 0 || n % 3 == 0) return 0;
  for (int i = 5; i * i <= n; i += 6)
    if (n % i == 0 || n % (i + 2) == 0) return 0;
  return 1;
}

int main()
{
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  int n;
  cin >> n;

  cout << n << (checkPrimeNumber(n) ? " is a prime number." : " is not a prime number.");

  return 0;
}
