#include <iostream>
#include <string>
using namespace std;

void findAllIndex(string s1, string s2)
{
  int len = 0, res[1000], lastIndex = 0;
  string s21 = s2.substr(0, 1);

  if (s1.find(s21) == string::npos)
  {
    cout << -1;
    return;
  }

  while (s1.find(s21) != string::npos)
  {
    int index = s1.find(s21);
    res[len++] = lastIndex + index;
    s1 = s1.substr(index + 1);
    lastIndex += index + 1;
  }

  for(int i = 0; i < len - 1; i++) cout << res[i] << " ";
  cout << res[len - 1];
}
