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

char lowercase(char c)
{
  return (c >= 'A' && c <= 'Z') ? c + 32 : c;
}

bool isCharInList(char c, string list)
{
  FE(ch, list)
    if (ch == lowercase(c)) return 1;
  return 0;
}

bool isGroupVowel(string s)
{
  string groupVowels[] = { "ae", "ai", "ao", "au", "ei", "eu", "iu", "oi", "ou", "ui" };

  FE(g, groupVowels)
    if (s == g) return 1;
  return 0;
}

bool isValidChar(char c)
{
  return isCharInList(c, "aeioupkhlmnw '");
}

bool isVowel(char c)
{
  return isCharInList(c, "aeiou");
}

bool isConsonant(char c)
{
  return isCharInList(c, "pkhlmn");
}

bool isSpecialChar(char c)
{
  return isCharInList(c, " '");
}

int main()
{
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  string word, res = "";
  getline(cin, word);

  int n = (int)word.size();

  FE(x, word)
    if (!isValidChar(x))
    {
      cout << word << " contains invalid characters.";
      return 0;
    }

  FO(i, n)
  {
    char curr = lowercase(word[i]), next = (i + 1 < n) ? lowercase(word[i + 1]) : 0;

    if (isSpecialChar(curr))
    {
      res += curr;
      continue;
    }

    if (isConsonant(curr))
    {
      res += curr;
      continue;
    }

    if (curr == 'w')
    {
      char prev = (i - 1 >= 0) ? word[i - 1] : 0;
      if (isCharInList(prev, "ie")) res += 'v';
      else res += 'w';
      continue;
    }

    if (isVowel(curr))
    {
      string s = "", pair = "";
      pair += curr;
      if (next) pair += next;

      if (isGroupVowel(pair))
      {
        if (pair == "ai" || pair == "ae") s = "eye";
        else if (pair == "ao" || pair == "au") s = "ow";
        else if (pair == "ei") s = "ay";
        else if (pair == "eu") s = "eh-oo";
        else if (pair == "iu") s = "ew";
        else if (pair == "oi") s = "oy";
        else if (pair == "ou") s = "ow";
        else if (pair == "ui") s = "ooey";

        i++;
        next = (i + 1 < n) ? word[i + 1] : 0;
      }
      else
      {
        if (curr == 'a') s = "ah";
        else if (curr == 'e') s = "eh";
        else if (curr == 'i') s = "ee";
        else if (curr == 'o') s = "oh";
        else if (curr == 'u') s = "oo";
      }
      res += s;
      if(next && !isSpecialChar(next)) res += '-';
    }
  }

  cout << res << el;

  return 0;
}
