#include <string>
using namespace std;

string repeatString(const string& s, int repeat)
{
  if (repeat < 1) return s;
  return s + repeatString(s, repeat - 1);
}

string solve(const string& s, int& i)
{
  if (i >= (int)s.size()) return "";

  if (s[i] >= '0' && s[i] <= '9')
  {
    int repeat = s[i] - '0';
    i += 2;
    string left = solve(s, i), right = solve(s, i);
    return repeatString(left, repeat) + right;
  }

  if (s[i] == ')')
  {
    i++;
    return "";
  }

  string curr = string(1, s[i]);
  return curr + solve(s, ++i);
}

string expand(string s)
{
  int i = 0;
  return solve(s, i);
}
