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

  int a, b, c;
  cin >> a >> b >> c;

  if (a == b && b == c)
    cout << "The triangle is Equilateral." << el;
  else if (a == b || b == c || a == c)
    cout << "The triangle is Isosceles." << el;
  else if (a * a + b * b == c * c || a * a + c * c == b * b || b * b + c * c == a * a)
    cout << "The triangle is Right-angled." << el;
  else
    cout << "The triangle is Scalene." << el;

  return 0;
}
