int strLen(char* str)
{
  if (!*str) return 0;
  return 1 + strLen(str + 1);
}
