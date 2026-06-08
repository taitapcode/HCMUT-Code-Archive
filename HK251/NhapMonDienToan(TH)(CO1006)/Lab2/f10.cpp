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

void swap(int &a, int &b)
{
  int temp = a;
  a = b;
  b = temp;
}

int main()
{
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  int x, y;
  cin >> x >> y;

  swap(x, y);
  cout << x << " " << y << el;

  return 0;
}
