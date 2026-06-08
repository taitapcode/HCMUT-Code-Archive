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

  int s;
  cin >> s;

  if (s < 0 || s > 100) cout << "Grade is not exist" << el;
  else if (s >= 90) cout << "A" << el;
  else if (s >= 80) cout << "B" << el;
  else if (s >= 70) cout << "C" << el;
  else if (s >= 60) cout << "D" << el;
  else cout << "F" << el;

  return 0;
}
