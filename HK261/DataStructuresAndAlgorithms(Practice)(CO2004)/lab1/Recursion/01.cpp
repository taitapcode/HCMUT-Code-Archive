#include <iostream>
using namespace std;
void printArray(int n)
{
  if (n) printArray(n - 1);
  cout << (n ? ", " : "") << n;
}
