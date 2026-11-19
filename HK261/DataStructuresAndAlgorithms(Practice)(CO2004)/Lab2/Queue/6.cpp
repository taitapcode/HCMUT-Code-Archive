#include <queue>
using namespace std;

void interleaveQueue(queue<int>& q)
{
  int half = q.size();
  queue<int> firstHalf;
  for (int i = 0; i < half / 2; i++)
  {
    firstHalf.push(q.front());
    q.pop();
  }

  while (!firstHalf.empty())
  {
    q.push(firstHalf.front());
    firstHalf.pop();
    q.push(q.front());
    q.pop();
  }
}
