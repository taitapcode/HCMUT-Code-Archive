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

  int a, b, c;
  cin >> a >> b >> c;

  int delta = b * b - 4 * a * c;

  db x1 = (-b + sqrt(delta)) / (2.0 * a), x2 = (-b - sqrt(delta)) / (2.0 * a);

  cout << fixed << setprecision(2) << "X1= " << x1 << "\nX2= " << x2;

  return 0;
}
