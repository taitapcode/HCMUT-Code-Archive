#include <string>
using namespace std;

string repeat(const string& s, int count)
{
  if (count <= 0) return "";
  return s + repeat(s, count - 1);
}

string helper(string s, int& i)
{
  if (i >= (int)s.size()) return "";

  if (s[i] >= '0' && s[i] <= '9')
  {
    int rcount = s[i] - '0';
    i += 2;
    string left = helper(s, i), right = helper(s, i);
    return repeat(left, rcount) + right;
  }

  if (s[i] == ')')
  {
    ++i;
    return "";
  }

  string curr = string(1, s[i]);
  ++i;
  return curr + helper(s, i);
}

string expand(string s)
{
  int idx = 0;
  return helper(s, idx);
}
