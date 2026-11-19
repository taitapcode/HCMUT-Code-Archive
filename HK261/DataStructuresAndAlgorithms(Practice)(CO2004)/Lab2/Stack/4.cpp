#include <bits/stdc++.h>
using namespace std;

string removeDuplicates(string S)
{
  stack<char> st;
  for (char ch : S)
  {
    if (!st.empty() && ch == st.top())
      st.pop();
    else
      st.push(ch);
  }

  string res;
  while (!st.empty())
  {
    res = st.top() + res;
    st.pop();
  }

  return res;
}
