char* concatStr(char* str1, char* str2)
{
  int n1 = 0, n2 = 0;
  while(str1[n1]) n1++;
  while(str2[n2]) n2++;

  char *concat = new char[n1 + n2 + 1];
  concat[n1 + n2] = 0;

  for(int i = 0; i < n1; i++) concat[i] = str1[i];
  for(int i = 0; i < n2; i++) concat[n1 + i] = str2[i];

  return concat;
}
