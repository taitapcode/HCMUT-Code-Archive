#include <cstring>

void recover(char str[])
{
  int len = strlen(str);
  for (int i = 0; i < len; i++)
  {
    if (str[i] >= 'a' && str[i] <= 'z') str[i] = 'A' + (str[i] - 'a');
    else if (str[i] >= 'A' && str[i] <= 'Z') str[i] = 'a' + (str[i] - 'A');
  }
}
