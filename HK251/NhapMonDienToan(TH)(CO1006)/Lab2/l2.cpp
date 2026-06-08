#include<bits/stdc++.h>
#include <iomanip>
#define el '\n'
#define ll long long
#define db double
#define ldb long double
#define FOR(i, a, b) for(int i = a; i < b; i++)
#define FO(i, n) for(int i = 0; i < n; i++)
#define FE(x, a) for (auto &x : a)
using namespace std;

int main()
{
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  db hourlyRates[] = {9.5, 6.4, 12.5, 5.5, 10.5}, workingHours, wages;

  cout << left << setw(30) << "Hourly Rate" << setw(30) << "Working Hour" << setw(30) << "Wage" << el;

  FO(i, 5)
  {
    cin >> workingHours;
    wages = hourlyRates[i] * workingHours;
    cout << left << fixed << setprecision(10)
      << setw(30) << hourlyRates[i] << setw(30) << workingHours << setw(30) << wages << el;
  }

  return 0;
}
