#include <string>
using namespace std;

string reverseSentence(string s)
{
  int index = s.find(' ');
  if (index == -1) return s;
  return reverseSentence(s.substr(index + 1)) + " " + s.substr(0, index);
}
