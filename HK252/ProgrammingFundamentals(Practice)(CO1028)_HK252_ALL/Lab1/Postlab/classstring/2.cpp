#include <iostream>
#include <string>

using namespace std;

int longestNonRepeatSubString(string s)
{
  int n = s.size(), ans = 0;
  bool v[26] = {};

  if (n < 2) return n;

  int l, r;
  for(r = l = 0; r < n; r++)
  {
    while (v[s[r] - 'a']) v[s[l++] - 'a'] = 0;
    v[s[r] - 'a'] = 1;

    ans = max(ans, r - l + 1);
  }

  return ans;
}

int main()
{
  string s;
  cin >> s;
  cout << longestNonRepeatSubString(s);
  return 0;
}
