#include <bits/stdc++.h>
using namespace std;

vector<int> stock_span(const vector<int>& ns)
{
  int n = ns.size();
  stack<int> st;
  vector<int> res(n, 1);

  for (int i = 0; i < n; i++)
  {
    while (!st.empty() && ns[st.top()] < ns[i]) st.pop();
    res[i] = st.empty() ? (i + 1) : (i - st.top());
    st.push(i);
  }

  return res;
}
