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

void EvenOrOdd()
{
  int num;
  cin >> num;

  cout << ((num & 1) ? "Odd" : "Even");
}

int main()
{
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  EvenOrOdd();

  return 0;
}
