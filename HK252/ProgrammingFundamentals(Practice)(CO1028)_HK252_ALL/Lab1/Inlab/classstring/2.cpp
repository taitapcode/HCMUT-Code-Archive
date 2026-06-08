#include <iostream>
#include <string>
using namespace std;

void deleteWord(string s, string s1)
{
  int pos = s.find(s1);
  while (pos > -1)
  {
    s.erase(pos, s1.length());
    pos = s.find(s1);
  }

  cout << s;
}
