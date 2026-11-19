#include <queue>
using namespace std;

long long nthNiceNumber(int n)
{
  long long res = 0, mul = 1;
  while (n > 0)
  {
    if (n % 2 == 1)
    {
      res += 2 * mul;
      n = (n - 1) / 2;
    }
    else
    {
      res += 5 * mul;
      n = (n - 2) / 2;
    }
    mul *= 10;
  }
  return res;
}

long long nthNiceNumber2(int n)
{
  queue<long long> q;
  q.push(2);
  q.push(5);
  long long cur = 0;
  for (int i = 0; i < n; i++)
  {
    cur = q.front();
    q.pop();
    q.push(cur * 10 + 2);
    q.push(cur * 10 + 5);
  }
  return cur;
}
