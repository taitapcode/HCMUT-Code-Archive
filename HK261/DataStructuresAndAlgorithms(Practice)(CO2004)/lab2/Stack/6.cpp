#include <stack>
#include <vector>
using namespace std;

class Node
{
public:
  int x, y;
  int dir;
  Node(int i, int j)
  {
    x = i;
    y = j;  // Initially direction
    dir = 0;
  }
};

bool canEatFood(int maze[5][5], int fx, int fy)
{
  if (fx >= 5 || fx < 0 || fy >= 5 || fy < 0) return false;
  if (!maze[0][0] || !maze[fx][fy]) return false;
  if (fx == 0 && fy == 0) return true;

  const int dx[4] = {-1, 0, 1, 0};
  const int dy[4] = {0, 1, 0, -1};

  stack<Node> st;
  vector<vector<bool>> visited(5, vector<bool>(5, false));

  visited[0][0] = true;
  st.push(Node(0, 0));
  while (!st.empty())
  {
    Node& top = st.top();
    int x = top.x, y = top.y, dir = top.dir;

    if (dir == 4)
    {
      st.pop();
      continue;
    }

    int d = top.dir++;
    int nx = x + dx[d], ny = y + dy[d];

    if (nx < 0 || nx >= 5 || ny < 0 || ny >= 5) continue;
    if (!maze[nx][ny] || visited[nx][ny]) continue;

    if (nx == fx && ny == fy) return true;
    visited[nx][ny] = true;

    st.push(Node(nx, ny));
  }

  return false;
}
