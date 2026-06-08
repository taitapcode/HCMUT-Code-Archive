#include "clocktype.h"

int clamp(int x, int max)
{
  if (x >= max || x < 0) return 0;
  return x;
}

void ClockType::setTime(int hour, int minute, int second)
{
  hr = clamp(hour, 24);
  min = clamp(minute, 60);
  sec = clamp(second, 60);
}
