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

db convertDegreeToRadian(db degree) {
    return degree * (3.14 / 180.0);
}

int main()
{
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  for(db degree = 5; degree <= 85; degree += 5) {
    db x = convertDegreeToRadian(degree);
    cout << fixed << sp(2) << sin(x) << ' ' << cos(x) << ' ' << tan(x) << el;
  }

  return 0;
}
