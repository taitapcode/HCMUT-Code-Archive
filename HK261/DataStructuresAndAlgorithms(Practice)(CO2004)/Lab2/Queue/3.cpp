#include <iostream>
#include <queue>
#include <vector>
using namespace std;

void bfs(vector<vector<int>> graph, int start)
{
  vector<bool> visited(graph.size(), false);
  queue<int> q;
  q.push(start);

  while (!q.empty())
  {
    int node = q.front();
    q.pop();

    if (visited[node]) continue;
    visited[node] = true;

    cout << node << " ";

    for (int next : graph[node])
      if (!visited[next]) q.push(next);
  }
}
