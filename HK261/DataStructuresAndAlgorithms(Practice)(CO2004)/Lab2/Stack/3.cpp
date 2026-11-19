#include <bits/stdc++.h>
using namespace std;

vector<int> nextGreater(vector<int>& arr)
{
  int n = arr.size();
  stack<int> st;
  vector<int> res(n, -1);

  for (int i = 0; i < n; i++)
  {
    while (!st.empty() && arr[st.top()] < arr[i])
    {
      res[st.top()] = arr[i];
      st.pop();
    }
    st.push(i);
  }

  return res;
}
