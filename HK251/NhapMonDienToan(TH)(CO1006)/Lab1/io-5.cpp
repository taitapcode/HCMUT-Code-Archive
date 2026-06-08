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

  int ry = y / 365, rw = (y % 365) / 7, rd = (y % 365) % 7;
  printf("%d days = %d years + %d weeks + %d days", y, ry, rw, rd);


  return 0;
}
