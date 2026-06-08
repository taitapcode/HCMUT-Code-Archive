#include <cstring>

int find(char str[], char substr[])
{
  int strLen = strlen(str), subLen = strlen(substr);

  for(int i = 0; i <= strLen - subLen; i++)
    for(int j = 0; j < subLen; j++)
    {
      if (str[i + j] != substr[j]) break;
      if (j == subLen - 1) return i;
    }

  return -1;
}
