#include <string>
using namespace std;

int helper(const string& s, int i, int scope)
{
  if (i >= (int)s.size()) return scope;

  if (s[i] == '(') return helper(s, i + 1, scope + 1);
  if (s[i] == ')' && scope > 0) return helper(s, i + 1, scope - 1);

  return 1 + helper(s, i + 1, 0);
}

int mininumBracketAdd(string s)
{
  return helper(s, 0, 0);
}
