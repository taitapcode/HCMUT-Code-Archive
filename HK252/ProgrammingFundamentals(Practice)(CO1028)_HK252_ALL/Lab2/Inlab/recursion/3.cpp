#include<iostream>
#include<string>
using namespace std;
/* END of library */
bool palindrome(string strg);
// {
//   int l = 0, r = strg.size() - 1;
//   while (l < r)
//   {
//     if (strg[l] != strg[r]) return 0;
//     l++, r--;
//   }
//
//   return 1;
// };

bool palindromeRecursion(string s)
{
    // BEGIN YOUR IMPLEMENTATION [1]
  if (s.size() < 2) return 1;
  return s.back() == s.front() && palindrome(s.substr(1, s.size() - 2));

    // END YOUR IMPLEMENTATION [1]
}
int main()
{
    // hiddenCheck();
    // BEGIN YOUR IMPLEMENTATION [2]

  string s;
  while (cin >> s)
  {
    if (s[0] == '*') break;

    cout << (palindrome(s) ? "true" : "false") << ' ' << (palindromeRecursion(s) ? "true" : "false") << '\n';
  }

    // END YOUR IMPLEMENTATION [2]
    return 0;
}
