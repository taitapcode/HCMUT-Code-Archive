#include <stack>
#include <string>
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
  if (!maze[0][0] || !maze[fx][fy]) return false;
  if (fx >= 5 || fx < 0 || fy >= 5 || fy < 0) return false;
  if (fx == 0 && fy == 0) return true;

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

    top.dir++;
  }
}
