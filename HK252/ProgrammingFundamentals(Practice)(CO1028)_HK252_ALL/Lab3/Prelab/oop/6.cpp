#include "clocktype.h"

int clamp(int x, int max)
{
  if (x >= max || x < 0) return 0;
  return x;
}

ClockType::ClockType(int hr, int min, int sec)
{
  this->hr = clamp(hr, 24);
  this->min = clamp(min, 60);
  this->sec = clamp(sec, 60);
}

ClockType::ClockType() : hr(0), min(0), sec(0) {}
