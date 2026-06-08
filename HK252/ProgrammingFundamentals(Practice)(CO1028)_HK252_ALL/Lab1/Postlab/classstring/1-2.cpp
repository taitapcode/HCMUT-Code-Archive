#include <iostream>
#include <string>
#define el '\n'
using namespace std;

int expand(const string& s, int left, int right)
{
  while(left >= 0 && right < s.size() && s[left] == s[right])
    left--, right++;

  return right - left - 1;
}

string solve(string s)
{
  if (s.empty()) return s;

  int maxLen = 0, start = 0;
  for(size_t i = 0; i < s.size(); i++)
  {
    int len = max(expand(s, i, i), expand(s, i, i + 1));

    if (len > maxLen)
    {
      maxLen = len;
      start = i - (len - 1) / 2;
    }
  }

  return s.substr(start, maxLen);
}

int main()
{
  string s;
  cin >> s;

  cout << solve(s);

  return 0;
}
