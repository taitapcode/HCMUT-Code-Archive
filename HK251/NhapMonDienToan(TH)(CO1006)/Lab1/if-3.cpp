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

  double a;
  cin >> a;

  if (a < 0 || a >= 360) cout << "not exist" << el;
  else if (a >= 0 && a < 90) cout << "first quadrant" << el;
  else if (a >= 90 && a < 180) cout << "second quadrant" << el;
  else if (a >= 180 && a < 270) cout << "third quadrant" << el;
  else if (a >= 270 && a < 360) cout << "fourth quadrant" << el;

  return 0;
}
