#include <iostream>
#include <string>
#define ll long long
#define db double
#define ldb long double
using namespace std;

string manacher(string str)
{
  if (str.empty()) return "";

  string s = "#";
  for(char &x: str) s += string(1, x) + "#";

  int c, r, mc, maxp, n = s.size();
  int *p = new int[n]{};
  c = r = mc = maxp = 0;

  for(int i = 0; i < n; i++)
  {
    int m = 2 * c - i;

    if (i < r) p[i] = min(p[m], r - i);
    while(i + 1 + p[i] < n && i - 1 - p[i] >= 0 && s[i + 1 + p[i]] == s[i - 1 - p[i]]) p[i]++;

    if (i + p[i] > r)
    {
      c = i;
      r = p[i] + i;
    }

    if (p[i] > maxp)
    {
      maxp = p[i];
      mc = i;
    }
  }

  delete []p;
  return str.substr((mc - maxp) / 2, maxp);
}

int main()
{
  string s;
  cin >> s;

  cout << manacher(s);

  return 0;
}
