#include <iostream>
#include <cstring>
using namespace std;

char words[1000][30];
void printFirstRepeatedWord(char str[])
{
  int idx = 0;
  char *currWord = strtok(str, " ");

  while(currWord != NULL)
  {
    for (int i = 0; i < idx; i++)
      if (strcmp(currWord, words[i]) == 0)
      {
        cout << currWord;
        return;
      }
    strcpy(words[idx++], currWord);
    currWord = strtok(NULL, " ");
  }

  cout << "No Repetition";
}
