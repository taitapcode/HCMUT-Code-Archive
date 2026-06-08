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

  char code;
  cin >> code;

  if (code == 'M' || code == 'm') cout << "Individual is married";
  else if (code == 'D' || code == 'd') cout << "Individual is divorced";
  else if (code == 'W' || code == 'w') cout << "Individual is widowed";
  else cout << "Invalid code";

  return 0;
}
