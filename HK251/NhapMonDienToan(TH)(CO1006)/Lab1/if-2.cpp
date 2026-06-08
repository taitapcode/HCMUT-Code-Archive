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
  cout << fixed << setprecision(4);

  double a, b, c;
  cin >> a >> b >> c;

  if (a == 0)
  {
    if (b == 0)
    {
      cout << (c == 0 ? "Countless solutions" : "No solution");
      return 0;
    }
    double x = -1.0 * c / b;
    cout << x;
    return 0;
  }

  double D = b * b - 4 * a * c;
  if (D < 0)
  {
    double x1_r = -b / (2 * a);
    double x1_i = sqrt(-D) / (2 * a);
    double x2_r = -b / (2 * a);
    double x2_i = -sqrt(-D) / (2 * a);
    cout << x1_r << " + " << x1_i << "*i" << el;
    if (x2_i >= 0) cout << x2_r << " + " << x2_i << "*i";
    else cout << x2_r << " - " << -x2_i << "*i";
    return 0;
  }

  double x1 = (-b + sqrt(D)) / (2 * a);
  double x2 = (-b - sqrt(D)) / (2 * a);

  if (x1 == x2) cout << x1;
  else cout << x1 << el << x2;

  return 0;
}
