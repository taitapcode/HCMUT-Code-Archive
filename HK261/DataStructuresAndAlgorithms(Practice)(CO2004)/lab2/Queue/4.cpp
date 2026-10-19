#include <queue>
#include <vector>
using namespace std;

bool isBipartite(vector<vector<int>> graph)
{
  int n = graph.size();
  vector<int> color(n, -1);
  queue<int> q;

  for (int s = 0; s < n; ++s)
  {
    if (color[s] != -1) continue;

    color[s] = 0;
    q.push(s);

    while (!q.empty())
    {
      int u = q.front();
      q.pop();

      for (int v : graph[u])
      {
        if (color[v] == -1)
        {
          color[v] = 1 - color[u];
          q.push(v);
        }
        else if (color[v] == color[u])
          return false;
      }
    }
  }
  return true;
}
