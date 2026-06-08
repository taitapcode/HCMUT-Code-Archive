#include <string>
#include <iostream>
#include <fstream>
using namespace std;

void process(string fileName)
{
  ifstream ifs(fileName);
  double n, m, x;
  ifs >> n >> m;
  double maxNum;
  bool check = 0;

  while (n--)
  {
    double maxRow;;
    for(int i = 0; i < m; i++)
    {
      ifs >> x;
      if (x > maxRow || i == 0) maxRow = x;
      if (x > maxNum || !check) maxNum = x;
      check = 1;
    }
    cout << maxRow << ' ';
  }
  cout << maxNum;
}


