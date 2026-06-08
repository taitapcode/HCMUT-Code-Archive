#include <bits/stdc++.h>
using namespace std;

string convertToBaseM(int n, int m = 2)
{
  string ans;

  while(n > 0)
  {
    ans = to_string(n % m) + ans;
    n /= m;
  }

  return ans;
}

int main()
{
  int n;
  cin >> n;
  cout << convertToBaseM(n, 4) << '\n';
  cout << convertToBaseM(n, 8) << '\n';
  cout << convertToBaseM(n);
  return 0;
}
