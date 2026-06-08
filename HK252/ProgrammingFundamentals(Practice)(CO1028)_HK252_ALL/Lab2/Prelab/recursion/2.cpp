#include<iostream>
using namespace std;

int gcdRecursion(int p, int q)
{
  if (q == 0) return p;
  return gcdRecursion(q, p % q);
}

int gcdIteration(int p, int q)
{
  while (q != 0)
  {
    int m = p % q;
    p = q;
    q = m;
  }

  return p;
}

int main()
{
    int p,q;
    cin>>p>>q;
    cout<<gcdRecursion(p,q)<< " "<<gcdIteration(p,q);
    return 0;
}
