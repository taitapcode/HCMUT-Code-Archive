#include <iostream>
using namespace std;

long int decimalToBinary(int decimal_number)
{
  if (!decimal_number) return 0;
  return decimal_number % 2 + 10 * decimalToBinary(decimal_number / 2);
}

int main()
{
  cout << decimalToBinary(20);

  return 0;
}
