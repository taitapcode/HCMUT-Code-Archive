#include <iostream>
#include <fstream>
using namespace std;

void calSum(string fileName)
{
  fstream fs(fileName, ios::in);
  int sum = 0, x;

  while(fs >> x)
    if (x > 0) sum += x;
  cout << sum;

  fs.close();
}
