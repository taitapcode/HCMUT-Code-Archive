int myArrayToInt(char* s, int n)
{
  return s[n - 1] - '0' + (n - 1 ? myArrayToInt(s, n - 1) * 10 : 0);
}
