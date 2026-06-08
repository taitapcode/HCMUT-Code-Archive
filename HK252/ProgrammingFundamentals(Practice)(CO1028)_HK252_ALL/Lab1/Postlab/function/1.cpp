#include <bits/stdc++.h>
using namespace std;

void mostFrequentCharacter(char* str, char& res, int& freq)
{
  int cnt[26] = {0};
  for(int i = 0; str[i]; i++)
  {
    if (str[i] >= 'A' && str[i] <= 'Z') str[i] += ' ';
    cnt[str[i] - 'a']++;
  }

  int ans = 0;
  for(int i = 1; i < 26; i++)
    if (cnt[ans] < cnt[i]) ans = i;

  res = (char)(ans + 'a');
  freq = cnt[ans];
}

int main()
{
  int n;
  cin >> n;
  char* str = new char[n+1];
  for(int i = 0; i < n; i++) cin >> str[i];
  str[n] = 0;
  char res = 0;
  int freq = 0;
  mostFrequentCharacter(str, res, freq);
  cout << res << ' ' << freq;
  delete[] str;

  return 0;
}
