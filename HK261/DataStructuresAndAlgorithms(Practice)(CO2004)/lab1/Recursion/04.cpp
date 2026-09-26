#include <string>
using namespace std;

bool check(const string& s, int l, int r)
{
  if (r <= l) return true;
  if (s[l] == ' ') return check(s, l + 1, r);
  if (s[r] == ' ') return check(s, l, r - 1);

  return s[l] == s[r] && check(s, l + 1, r - 1);
}

bool isPalindrome(string str)
{
  if (str.size() < 2) return true;
  return check(str, 0, str.size() - 1);
}
