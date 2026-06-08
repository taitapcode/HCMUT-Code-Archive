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

int getMaxElement(int a[], int n)
{
  int mx = a[0];
  FOR(i, 1, n) mx = max(mx, a[i]);
  return mx;
}

int main()
{
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  int n;
  cin >> n;
  int a[n];

  FE(x, a) cin >> x;
  cout << getMaxElement(a,n);

  return 0;
}
