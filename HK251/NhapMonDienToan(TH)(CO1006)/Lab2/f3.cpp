#include<bits/stdc++.h>
#define el '\n'
#define ll long long
#define db double
#define ldb long double
#define FOR(i, a, b) for(int i = a; i < b; i++)
#define FO(i, n) for(int i = 0; i < n; i++)
#define FE(x, a) for (auto &x : a)
#define sp(n) setprecision(n)
using namespace std;

double getDiameter(double radius)
{
  return 2 * radius;
}

double getCircumference(double radius)
{
   return 2 * 3.14 * radius;
}

double getArea(double radius)
{
   return 3.14 * radius * radius;
}

int main()
{
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  double radius;
  cout << "Enter the radius of the circle: ";
  cin >> radius;

  double diameter = getDiameter(radius);
  double circumference = getCircumference(radius);
  double area = getArea(radius);

  cout << "Diameter of the circle: " << diameter << " units" << el;
  cout << "Circumference of the circle: " << circumference << " units" << el;
  cout << "Area of the circle: " << area << " sq. units" << el;

  return 0;
}

