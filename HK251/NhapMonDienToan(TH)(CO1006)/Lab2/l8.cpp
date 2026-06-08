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
  db x, mx = -1e9;

  cin >> n;
  FO(i, n)
  {
    cin >> x;
    mx = max(mx, x);
  }

  cout << fixed << sp(2) << mx;

  return 0;
}
