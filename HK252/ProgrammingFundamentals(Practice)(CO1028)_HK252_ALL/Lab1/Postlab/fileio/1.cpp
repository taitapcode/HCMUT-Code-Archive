#include <bits/stdc++.h>
#define el '\n'
#define ll long long
#define db double
#define ldb long double
using namespace std;

void studentGrading(string fileName)
{
  ifstream fs(fileName);
  int n, grade[4] = {};
  double s1, s2, s3, s4;

  fs >> n;

  while(n--)
  {
    fs >> s1 >> s2 >> s3 >> s4;
    double avg = (s1 + s2 + s3 + s4) / 4;

    if (s1 < 5 || s2 < 5 || s3 < 5 || s4 < 5) grade[3]++;
    else if (avg >= 8) grade[0]++;
    else if (avg >= 6.5) grade[1]++;
    else if (avg >= 5) grade[2]++;
    else grade[3]++;
  }

  for(int i = 0; i < 4; i++)
    cout << (char)('A' + i) << ' ' << grade[i] << '\n';
}

int main()
{
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  studentGrading("a.txt");

  return 0;
}
