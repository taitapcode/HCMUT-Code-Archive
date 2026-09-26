#include <iostream>
using namespace std;

void printHailstone(int n)
{
  if (n < 1) return;
  cout << n;
  if (n == 1) return;

  cout << ' ';

  if (n & 1)
    printHailstone(n * 3 + 1);
  else
    printHailstone(n / 2);
}
