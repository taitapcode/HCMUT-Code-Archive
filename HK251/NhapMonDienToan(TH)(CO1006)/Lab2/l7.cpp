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

int main()
{
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  int n;
  cin >> n;

  ll last = 0, next = 1;
  cout << last << ' ';
  FO(i, n - 1)
  {
    ll curr = last + next;
    last = next;
    next = curr;

    cout << last << ' ';
  }

  return 0;
}
