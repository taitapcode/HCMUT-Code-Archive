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

  string name, sex;
  int age, weight;
  db height;

  cin >> name >> age >> sex >> height >> weight;

  cout << "Name: " << name << el;
  cout << "Age: " << age << el;
  cout << "Sex: " << sex << el;
  cout << "Height: " << fixed << setprecision(2) << height << el;
  cout << "Weight: " << weight << el;

  return 0;
}
