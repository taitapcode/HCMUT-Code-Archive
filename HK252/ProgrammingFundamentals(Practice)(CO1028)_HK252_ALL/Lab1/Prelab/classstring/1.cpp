#include <iostream>
#include <string>
using namespace std;

void cutString(string s, int index)
{
  int sLen = s.length();
  if (index < 0 || index >= sLen) return;
  cout << s.substr(index) << endl;
}
