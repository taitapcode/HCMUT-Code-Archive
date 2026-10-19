#include <queue>
#include <vector>
using namespace std;

struct Position
{
  int x, y;
};

int secondsToBeRotten(vector<vector<int>>& grid)
{
  int n = grid.size(), m = grid[0].size();
  int fresh = 0, mins = 0;
  queue<Position> q;

  for (int i = 0; i < n; i++)
    for (int j = 0; j < m; j++)
    {
      if (grid[i][j] == 2)
        q.push({i, j});
      else if (grid[i][j] == 1)
        fresh++;
    }

  const int dx[] = {-1, 0, 1, 0};
  const int dy[] = {0, 1, 0, -1};

  while (!q.empty())
  {
    int newRotten = 0, queSize = q.size();
    for (int i = 0; i < queSize; i++)
    {
      Position pos = q.front();
      q.pop();

      for (int i = 0; i < 4; i++)
      {
        int nx = pos.x + dx[i], ny = pos.y + dy[i];
        if (nx >= 0 && nx < n && ny >= 0 && ny < m && grid[nx][ny] == 1)
        {
          grid[nx][ny] = 2;
          q.push({nx, ny});
          fresh--;
          newRotten++;
        }
      }
    }

    if (newRotten > 0) mins++;
  }

  return !fresh ? mins : -1;
}
