#include <iostream>
using namespace std;

int** readArray()
{
  int **a = new int*[10];

  for(int i = 0; i < 10; i++)
  {
    a[i] = new int[10]{};

    for(int j = 0; j < 10; j++)
    {
      cin >> a[i][j];
      if (!a[i][j]) break;
    }
  }

  return a;
}

int main()
{
  int **a = readArray();
  for(int i = 0; i < 10; i++)
  {
    for(int j = 0; j < 10; j++) cout << a[i][j] << ' ';
    cout << '\n';
  }

  return 0;
}
