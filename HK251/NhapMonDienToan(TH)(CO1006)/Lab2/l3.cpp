#include<bits/stdc++.h>
#define el '\n'
#define ll long long
#define db double
#define ldb long double
#define FOR(i, a, b) for(int i = a; i < b; i++)
#define FO(i, n) for(int i = 0; i < n; i++)
#define FE(x, a) for (auto &x : a)
#define sp(i) setprecision(i)
#define sw(i) setw(i)
using namespace std;

struct Student{
    char name[20];
    long int rollno;
    char sex;
    float height;
    float weight;
};


int main()
{
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  int n;
  cout << "How many names ?" << el;
  cin >> n;

  Student s[100];
  db avgHeight = 0.0, avgWeight = 0.0;
  FO(i, n)
  {
    cout << "record = "<< i+1 << el;
    cout << "name : "; cin>> s[i].name;
    cout << "rollno : "; cin>> s[i].rollno;
    cout << "sex : "; cin>> s[i].sex;
    cout << "height : "; cin>> s[i].height;
    cout << "weight : "; cin>> s[i].weight;
    cout << el;

    avgHeight += s[i].height;
    avgWeight += s[i].weight;
  }

  avgHeight /= n * 1.0;
  avgWeight /= n * 1.0;

  cout << left << sw(20) << "Name" << sw(20) << "Rollno" << sw(20) << "Sex" << sw(20) << "Height" << sw(20) << "Weight" << el;
  FO(i, n)
    cout << left << sw(20) << s[i].name << sw(20) << s[i].rollno << sw(20) << s[i].sex << fixed << sp(2) << sw(20) << s[i].height << sw(20) << s[i].weight << el;

  cout << fixed << sp(5) << avgHeight - 1e-5 << ' ' << avgWeight << el;

  return 0;
}
