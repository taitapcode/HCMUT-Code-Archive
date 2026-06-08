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

  int mtx[n][n];

  FO(i, n)
    FO(j, n)
      cin >> mtx[i][j];

  FO(i, n)
  {
    FO(j, n) cout << mtx[j][i] << ' ';
    cout << el;
  }

  return 0;
}
