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

  int y;
  cin >> y;

  if (y % 4 == 0 && (y % 100 != 0 || y % 400 == 0))
    cout << y << " is a leap year." << el;
  else
    cout << y << " is not a leap year." << el;

  return 0;
}
