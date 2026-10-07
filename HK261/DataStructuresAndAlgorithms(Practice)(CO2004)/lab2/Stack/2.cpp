#include <bits/stdc++.h>
using namespace std;

int baseballScore(string ops)
{
  stack<int> st;
  for (const char& op : ops)
  {
    if (op >= '0' && op <= '9')
      st.push(op - '0');
    else if (op == '+')
    {
      int a = st.top();
      st.pop();
      int b = st.top();
      st.push(a);
      st.push(a + b);
    }
    else if (op == 'C')
      st.pop();
    else if (op == 'D')
      st.push(st.top() * 2);
  }

  int sum = 0;
  while (!st.empty())
  {
    sum += st.top();
    st.pop();
  }

  return sum;
}
