#include<cstring>

void encrypt(char* text, int shift)
{
  int len = strlen(text);
  shift = (shift % 26 + 26) % 26;

  for (int i = 0; i < len; i++)
    if (text[i] >= 'a' && text[i] <= 'z') text[i] = (text[i] - 'a' + shift) % 26 + 'a';
    else if (text[i] >= 'A' && text[i] <= 'Z') text[i] = (text[i] - 'A' + shift) % 26 + 'A';
}

void decrypt(char* text, int shift)
{
  encrypt(text, -shift);
}
