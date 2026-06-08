#include "clocktype.h"

// TODO
int clamp(int x, int max)
{
  if (x >= max || x < 0) return 0;
  return x;
}

void ClockType::getTime(int &h, int &m, int &s) const
{
  h = clamp(this->hr, 24);
  m = clamp(this->min, 60);
  s = clamp(this->sec, 60);
}
