#include<cstring>

bool isPalindrome(const char* str)
{
  int len = strlen(str);
  for (int i = 0; i < len / 2; i++)
    if (str[i] != str[len - i - 1]) return 0;
  return 1;
}
