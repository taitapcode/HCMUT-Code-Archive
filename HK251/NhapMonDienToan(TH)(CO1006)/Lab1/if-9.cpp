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
  cin >> n;

  if (n > 0)
    cout << n << " is a positive number." << el;
  else if (n < 0)
    cout << n << " is a negative number." << el;
  // else
  //   cout << n << " is zero." << el;

  return 0;
}
