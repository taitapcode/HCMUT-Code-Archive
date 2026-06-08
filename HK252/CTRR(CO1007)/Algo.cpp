#include "Algo.h"

// Task 1
double h1(double adjMatrix[100][100], int curr, int goal)
{
  static std::vector<double> dist(100, 1e9);
  static int lastGoal = -1;

  if (goal != lastGoal)
  {
    std::fill(dist.begin(), dist.end(), 1e9);
    dist[goal] = 0;

    std::vector<int> que;
    que.push_back(goal);
    int head = 0;

    while (head < que.size())
    {
      int c = que[head++];
      for (int n = 0; n < 100; n++)
        if (adjMatrix[n][c] > 0 && dist[n] > dist[c] + 1)
        {
          dist[n] = dist[c] + 1;
          que.push_back(n);
        }
    }
    lastGoal = goal;
  }

  return dist[curr];
}

PathNode* findSocialPath(double adjMatrix[100][100], int startPerson, int goalPerson)
{
  std::vector<double> g(100, 1e9), f(100, 1e9);
  std::vector<int> parent(100, -1);
  std::vector<bool> visited(100, false);

  g[startPerson] = 0;
  f[startPerson] = h1(adjMatrix, startPerson, goalPerson);

  std::vector<Node> os;
  os.push_back({startPerson, f[startPerson], g[startPerson]});
  bool found = false;

  while (!os.empty())
  {
    Node node = pop(os);
    int curr = node.index;

    if (visited[curr]) continue;
    visited[curr] = true;

    if (curr == goalPerson)
    {
      found = true;
      break;
    }

    for (int next = 0; next < 100; next++)
      if (adjMatrix[curr][next] && !visited[next])
      {
        double new_g = g[curr] + 1;
        if (new_g < g[next])
        {
          g[next] = new_g;
          f[next] = new_g + h1(adjMatrix, next, goalPerson);
          parent[next] = curr;
          os.push_back({next, f[next], g[next]});
        }
      }
  }

  if (!found) return nullptr;

  PathNode* path = nullptr;
  int currIdx = goalPerson;

  while (currIdx != -1)
  {
    PathNode* newNode = new PathNode(std::to_string(currIdx), f[currIdx],
                                     g[currIdx], f[currIdx] - g[currIdx], path);
    path = newNode;
    currIdx = parent[currIdx];
  }

  return path;
}

// Task 2
double h2(int coords[100][2], int curr, int goal, int mode)
{
  double dx = std::abs(coords[curr][0] - coords[goal][0]);
  double dy = std::abs(coords[curr][1] - coords[goal][1]);

  if (mode == 1) return dx + dy;
  if (mode == 2) return std::sqrt(dx * dx + dy * dy);
  if (mode == 3) return std::max(dx, dy);

  return 0;
}

PathNode* findDronePath(double weightMatrix[100][100], int coords[100][2], int startPoint, int goalPoint, int mode)
{
  std::vector<double> g(100, 1e9), f(100, 1e9);
  std::vector<int> parent(100, -1);
  std::vector<bool> visited(100, false);

  g[startPoint] = 0;
  f[startPoint] = h2(coords, startPoint, goalPoint, mode);

  std::vector<Node> os;
  os.push_back({startPoint, f[startPoint], g[startPoint]});
  bool found = false;

  while (!os.empty())
  {
    Node node = pop(os);
    int curr = node.index;

    if (visited[curr]) continue;
    visited[curr] = true;

    if (curr == goalPoint)
    {
      found = true;
      break;
    }

    for (int next = 0; next < 100; next++)
    {
      if (weightMatrix[curr][next] > 0 && !visited[next])
      {
        double new_g = g[curr] + weightMatrix[curr][next];
        if (new_g < g[next])
        {
          g[next] = new_g;
          f[next] = new_g + h2(coords, next, goalPoint, mode);
          parent[next] = curr;
          os.push_back({next, f[next], g[next]});
        }
      }
    }
  }

  if (!found) return nullptr;

  PathNode* path = nullptr;
  int currIdx = goalPoint;

  while (currIdx != -1)
  {
    std::string nodeName = "(" + std::to_string(coords[currIdx][0]) + "," + std::to_string(coords[currIdx][1]) + ")";
    PathNode* newNode = new PathNode(nodeName, f[currIdx], g[currIdx], f[currIdx] - g[currIdx], path);
    path = newNode;
    currIdx = parent[currIdx];
  }

  return path;
}

// Task 3
double h3(int x1, int y1, int x2, int y2, int mode)
{
  double dx = std::abs(x1 - x2);
  double dy = std::abs(y1 - y2);

  if (mode == 1) return dx + dy;
  if (mode == 2) return std::max(dx, dy);

  return 0;
}

PathNode* findWarehousePath(int warehouse[100][100], int m, int n, int startX, int startY, int goalX, int goalY, int mode)
{
  std::vector<double> g(10005, 1e9), f(10005, 1e9);
  std::vector<int> parent(10005, -1);
  std::vector<bool> visited(10005, false);

  int start = startX * 100 + startY;
  int goal = goalX * 100 + goalY;

  g[start] = 0;
  f[start] = h3(startX, startY, goalX, goalY, mode);

  std::vector<Node> os;
  os.push_back({start, f[start], g[start]});
  bool found = false;

  int dx[] = {-1, 1, 0, 0, -1, -1, 1, 1};
  int dy[] = {0, 0, -1, 1, -1, 1, -1, 1};

  while (!os.empty())
  {
    Node node = pop(os);
    int curr = node.index;

    if (visited[curr]) continue;
    visited[curr] = true;

    if (curr == goal)
    {
      found = true;
      break;
    }

    int cx = curr / 100, cy = curr % 100;
    for (int i = 0; i < 8; i++)
    {
      int nx = cx + dx[i], ny = cy + dy[i];
      if (nx >= 0 && nx < m && ny >= 0 && ny < n && warehouse[nx][ny] == 0)
      {
        int nextIdx = nx * 100 + ny;
        if (!visited[nextIdx])
        {
          double cost = (i >= 4) ? 1.5 : 1.0;
          double new_g = g[curr] + cost;

          if (new_g < g[nextIdx])
          {
            g[nextIdx] = new_g;
            f[nextIdx] = new_g + h3(nx, ny, goalX, goalY, mode);
            parent[nextIdx] = curr;
            os.push_back(Node(nextIdx, f[nextIdx], g[nextIdx]));
          }
        }
      }
    }
  }
  if (!found) return nullptr;

  PathNode* path = nullptr;
  int currIdx = goal;

  while (parent[currIdx] != -1)
  {
    int cx = currIdx / 100, cy = currIdx % 100;
    int pIdx = parent[currIdx];
    int px = pIdx / 100, py = pIdx % 100;
    int diffX = cx - px, diffY = cy - py;
    std::string nodeName;

    if (diffX == -1 && diffY == 0)
      nodeName = "Up";
    else if (diffX == 1 && diffY == 0)
      nodeName = "Down";
    else if (diffX == 0 && diffY == -1)
      nodeName = "Left";
    else if (diffX == 0 && diffY == 1)
      nodeName = "Right";
    else if (diffX == -1 && diffY == -1)
      nodeName = "Up-Left";
    else if (diffX == -1 && diffY == 1)
      nodeName = "Up-Right";
    else if (diffX == 1 && diffY == -1)
      nodeName = "Down-Left";
    else if (diffX == 1 && diffY == 1)
      nodeName = "Down-Right";

    PathNode* newNode = new PathNode(nodeName, f[currIdx], g[currIdx],
                                     h3(cx, cy, goalX, goalY, mode), path);
    path = newNode;
    currIdx = pIdx;
  }

  return path;
}

// Task 4
double h4(int x1, int y1, int x2, int y2, int mode)
{
  double dx = std::abs(x1 - x2);
  double dy = std::abs(y1 - y2);

  if (mode == 1) return dx + dy;
  if (mode == 2) return std::max(dx, dy);

  return 0;
}

PathNode* findEvacuationPath(int floorPlan[100][100], int m, int n, int startX, int startY,
                             int exitX, int exitY, double weightMatrix[100][100], int mode)
{
  for (int i = 0; i < 100; i++)
    for (int j = 0; j < 100; j++) weightMatrix[i][j] = 0;

  int dx[] = {-1, 1, 0, 0, -1, -1, 1, 1};
  int dy[] = {0, 0, -1, 1, -1, 1, -1, 1};

  for (int r = 0; r < m; r++)
    for (int c = 0; c < n; c++)
      if (floorPlan[r][c] == 0)
      {
        int u = r * n + c;
        for (int i = 0; i < 8; i++)
        {
          int nr = r + dx[i], nc = c + dy[i];
          if (nr >= 0 && nr < m && nc >= 0 && nc < n && floorPlan[nr][nc] == 0)
          {
            int v = nr * n + nc;
            weightMatrix[u][v] = (i >= 4) ? 1.5 : 1.0;
          }
        }
      }

  std::vector<double> g(105, 1e9), f(105, 1e9);
  std::vector<int> parent(105, -1);
  std::vector<bool> visited(105, false);

  int start = startX * n + startY;
  int goal = exitX * n + exitY;

  g[start] = 0;
  f[start] = h4(startX, startY, exitX, exitY, mode);

  std::vector<Node> os;
  os.push_back({start, f[start], g[start]});
  bool found = false;

  while (!os.empty())
  {
    Node node = pop(os);
    int curr = node.index;

    if (visited[curr]) continue;
    visited[curr] = true;

    if (curr == goal)
    {
      found = true;
      break;
    }

    for (int nextIdx = 0; nextIdx < m * n; nextIdx++)
    {
      if (weightMatrix[curr][nextIdx] > 0 && !visited[nextIdx])
      {
        double new_g = g[curr] + weightMatrix[curr][nextIdx];
        if (new_g < g[nextIdx])
        {
          g[nextIdx] = new_g;
          int nr = nextIdx / n, nc = nextIdx % n;
          f[nextIdx] = new_g + h4(nr, nc, exitX, exitY, mode);
          parent[nextIdx] = curr;
          os.push_back(Node(nextIdx, f[nextIdx], g[nextIdx]));
        }
      }
    }
  }

  if (!found) return nullptr;

  PathNode* path = nullptr;
  int currIdx = goal;

  while (currIdx != -1)
  {
    int cx = currIdx / n, cy = currIdx % n;
    std::string nodeName = "(" + std::to_string(cx) + ", " + std::to_string(cy) + ")";

    PathNode* newNode = new PathNode(nodeName, f[currIdx], g[currIdx],
                                     f[currIdx] - g[currIdx], path);
    path = newNode;
    currIdx = parent[currIdx];
  }

  return path;
}
