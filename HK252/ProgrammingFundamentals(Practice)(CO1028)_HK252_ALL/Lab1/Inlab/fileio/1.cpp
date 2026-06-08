#include <string>
#include <iostream>
#include <fstream>
using namespace std;

void uppercase(string output)
{
  string input;
  cin >> input;

  for (char &x : input)
    if (x >= 'a' && x <= 'z') x = x - ('a' - 'A');

  ofstream outFile(output);
  outFile << input;
}
