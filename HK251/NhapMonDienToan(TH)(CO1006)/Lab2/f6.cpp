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

void printPrimes(int lower, int upper)
{
  bool prime[upper + 1];
  prime[0] = prime[1] = 0;
  for(int i = 2; i <= upper; i++) prime[i] = 1;
  for (int i = 2; i * i <= upper; i++)
    if (prime[i])
      for (int j = i * i; j <= upper; j += i) prime[j] = 0;

  for (int i = lower; i <= upper; i++)
    if (prime[i]) cout << i << " ";
}

int main()
{
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  int lb, ub;
  cin >> lb >> ub;
  printPrimes(lb, ub);

  return 0;
}
