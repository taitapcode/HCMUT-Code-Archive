#include <cstring>

void process(char str[], char outstr[])
{
  int idx = 0, len = strlen(str);
  for (int i = 0; i < len; i++)
  {
    if (i == 0 && str[i] == ' ') continue;
    if (str[i] == ' ' && str[i - 1] == ' ') continue;
    outstr[idx++] = str[i];
  }
  outstr[idx] = '\0';
}
