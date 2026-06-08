#include <bits/stdc++.h>
using namespace std;

int price[100000];
void calMoney(string price, string buy)
{
  ifstream pin(price), bin(buy);
  int n;

  pin >> n;
  pin.ignore();
  while(n--)
  {
    int id, p;
    pin >> id >> p;
    price[id] = p;
  }

  int m;
  string line, customer;
  bin >> m;
  bin.ignore();

  while(m--)
  {
    getline(bin, line);
    stringstream ss(line);

    ss >> customer;

    int id, quantity, amount = 0;
    while (ss >> id >> quantity) amount += price[id] * quantity;

    cout << customer << ' ' << amount << '\n';
  }
}

int main()
{
  calMoney("price.txt", "buy.txt");

  return 0;
}
