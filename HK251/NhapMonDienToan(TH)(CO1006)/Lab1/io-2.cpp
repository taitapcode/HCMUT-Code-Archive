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

  int a = 0, b;
  for(int i = 0; i < 5; i++) cin >> b, a+= b;
  cout << fixed << setprecision(2) << "The average of 5 integers: " << a/5.0;

  return 0;
}
