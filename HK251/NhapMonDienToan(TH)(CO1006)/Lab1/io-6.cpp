#include<bits/stdc++.h>
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

  double x[2], y[2];
  cin >> x[0] >> y[0] >> x[1] >> y[1];

  int manhattan_distance = abs(x[0] - x[1]) + abs(y[0] - y[1]);
  double euclidean_distance = sqrt(pow(x[0] - x[1], 2) + pow(y[0] - y[1], 2));

  cout << "Manhattan distance: " << manhattan_distance << el;
  cout << "Euclidean distance: " << fixed << setprecision(2) << euclidean_distance << el;

  return 0;
}
