#include<bits/stdc++.h>
#define el '\n'
#define ll long long
#define db double
#define ldb long double
#define FOR(i, a, b) for(int i = a; i < b; i++)
#define FO(i, n) for(int i = 0; i < n; i++)
#define FE(x, a) for (auto &x : a)
using namespace std;

int main()
{
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  int n;
  db x, avg = 0;

  cin >> n;
  FO(i, n)
  {
    cin >> x;
    avg += x;
  }
  avg /= n;
  cout << fixed << setprecision(2) << avg << el;

  return 0;
}
