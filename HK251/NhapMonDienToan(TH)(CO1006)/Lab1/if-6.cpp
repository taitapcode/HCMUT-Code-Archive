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

  int m;
  cin >> m;

  if (m == 1)
    cout << 31 << el;
  else if (m == 2)
    cout << "28 or 29" << el;
  else if (m == 3)
    cout << 31 << el;
  else if (m == 4)
    cout << 30 << el;
  else if (m == 5)
    cout << 31 << el;
  else if (m == 6)
    cout << 30 << el;
  else if (m == 7)
    cout << 31 << el;
  else if (m == 8)
    cout << 31 << el;
  else if (m == 9)
    cout << 30 << el;
  else if (m == 10)
    cout << 31 << el;
  else if (m == 11)
    cout << 30 << el;
  else if (m == 12)
    cout << 31 << el;

  return 0;
}
