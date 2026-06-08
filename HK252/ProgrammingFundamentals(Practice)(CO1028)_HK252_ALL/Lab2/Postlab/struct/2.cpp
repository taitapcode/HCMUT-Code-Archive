#include "struct.h"

SCP** sortDatabase(SCP** arr, int n)
{
  for(int i = 1; i < n; i++)
    for(int j = 0; j < i; j++)
      if (arr[i]->id < arr[j]->id)
      {
        SCP *temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
      }

  return arr;
}
