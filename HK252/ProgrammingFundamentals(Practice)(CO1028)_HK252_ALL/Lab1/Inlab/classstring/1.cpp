#include <iostream>
#include <string>
using namespace std;

void replaceString(string s, string s1, string s2)
{
  // int pos = s.rfind(s1);
  // if (pos != string::npos) s.replace(pos, s1.length(), s2);
  // cout << s;
  int curr = s.find(s1), last = curr;
  while (curr > -1)
  {
    last = curr;
    curr = s.find(s1, last + s1.length());
  }
  if (last > -1) s.replace(last, s1.length(), s2);
  cout << s;

}
