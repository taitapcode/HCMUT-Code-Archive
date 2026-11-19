#include <string>
using namespace std;

int solve(const string& s, int i, int scope)
{
  if (i >= (int)s.size()) return scope;
  if (i == '(') return solve(s, i + 1, scope + 1);
  if (i == ')' && scope > 0) return solve(s, i + 1, scope - 1);

  return 1 + solve(s, i + 1, 0);
}

int mininumBracketAdd(string s)
{
  return solve(s, 0, 0);
}
