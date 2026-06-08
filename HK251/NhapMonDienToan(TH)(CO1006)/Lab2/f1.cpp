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

int convertBinaryToDecimal(string n)
{
  int decimalValue = 0, base = 1;

  for (int i = n.length() - 1; i >= 0; i--)
  {
    decimalValue += base * (n[i] - '0');
    base *= 2;
  }

  return decimalValue;
}

int main()
{
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  string n;
  cin >> n;

  cout << n << " in binary = " << convertBinaryToDecimal(n) << " in decimal";

  return 0;
}
