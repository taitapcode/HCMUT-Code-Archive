#include <bits/stdc++.h>
using namespace std;

int evaluatePostfix(string expr)
{
  const string operators = "+-*/";
  stack<int> st;
  int n = expr.size();

  for (int i = 0; i < n;)
  {
    if (expr[i] == ' ')
    {
      i++;
      continue;
    }

    int start = i;
    while (expr[i] != ' ' && i < n) i++;
    string token = expr.substr(start, i - start);

    if (operators.find(token) != string::npos)
    {
      int b = st.top();
      st.pop();
      int a = st.top();
      st.pop();

      if (token == "+")
        st.push(a + b);
      else if (token == "-")
        st.push(a - b);
      else if (token == "*")
        st.push(a * b);
      else if (token == "/")
        st.push(a / b);
    }
    else
      st.push(stoi(token));
  }

  return st.top();
}
