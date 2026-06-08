#include "struct.h"

int compareObjectClass(const SCP &objA, const SCP &objB)
{
  if (objA.objClass < objB.objClass) return -1;
  if (objA.objClass == objB.objClass) return 0;
  return 1;
}
