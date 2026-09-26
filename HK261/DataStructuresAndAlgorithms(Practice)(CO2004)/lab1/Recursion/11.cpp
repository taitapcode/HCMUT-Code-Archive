#include <string>
using namespace std;

string reverseSentence(string s)
{
  int space_pos = s.find(' ');
  if (space_pos == -1) return s;

  return reverseSentence(s.substr(space_pos + 1)) + s.substr(0, space_pos - 1);
}
