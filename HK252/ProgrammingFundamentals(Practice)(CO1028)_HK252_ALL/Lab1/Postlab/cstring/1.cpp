inline char upper(char x)
{
  if (x >= 'a' && x <= 'z') x -= ' ';
  return x;
}

inline char lower(char x)
{
  if (x >= 'A' && x <= 'Z') x += ' ';
  return x;
}

inline bool isAlphabet(char x)
{
  return (x >= 'a' && x <= 'z') || (x >= 'A' && x <= 'Z');
}

void process(const char *name, char *outstr)
{
  int o = 0, i = 0;
  while (name[i] && !isAlphabet(name[i])) i++;
  for(; name[i]; i++)
    if (isAlphabet(name[i]))
    {
      if (!o || (o && outstr[o - 1] == ' ')) outstr[o++] = upper(name[i]);
      else outstr[o++] = lower(name[i]);
    }
    else if (name[i] == ' ' && outstr[o - 1] != ' ') outstr[o++] = ' ';

  if (o > 0 && outstr[o - 1] == ' ') o--;
  outstr[o] = 0;
}
