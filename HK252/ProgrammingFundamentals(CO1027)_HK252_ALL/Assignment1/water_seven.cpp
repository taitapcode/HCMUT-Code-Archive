#include "water_seven.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

int getCharIdx(const char *name, char character[FIXED_CHARACTER][MAX_NAME])
{
  for(int i = 0; i < FIXED_CHARACTER; i++)
    if (character[i][0] && !strcmp(name, character[i])) return i;

  return -1;
}

int rnd(double x) { return ceil(x); }

int sumDigit(int n)
{
  int s = 0;
  while (n) s += n % 10, n /= 10;
  return s;
}

bool isPerfectNum(int n)
{
  if (n < 2) return false;
  int s = 1;
  for(int i = 2; i * i <= n; i++)
    if (n % i == 0)
    {
      s += i;
      if (i * i != n) s += n / i;
    }

  return s == n;
}


//Task 0
bool readInput(
  const string &filename,
  char character[FIXED_CHARACTER][MAX_NAME], int hp[FIXED_CHARACTER],
  int skill[FIXED_CHARACTER], int &shipHP, int &repairCost
) {
  ifstream fs(filename);
  char name[MAX_NAME];
  int charHp, charSkill, l = 0;

  if (!fs.is_open()) return false;

  while(fs >> name >> charHp >> charSkill)
  {
    charHp = max(charHp, 0);
    charSkill = max(charSkill, 0);

    if (!strcmp(name, "GOING_MERRY"))
    {
      shipHP = min(charHp, 1000);
      repairCost = min(charSkill, 3000);
      continue;
    }

    charHp = min(charHp, 1000);
    charSkill = min(charSkill, 100);

    bool check = false;
    for(int i = 0; i < l; i++)
      if (!strcmp(character[i], name))
      {
        hp[i] = charHp;
        skill[i] = charSkill;
        check = true;
        break;
      }

    if (!check)
    {
      if (l >= FIXED_CHARACTER) return false;

      strcpy(character[l], name);
      hp[l] = charHp;
      skill[l] = charSkill;
      l++;
    }
  }

  return true;
}

// Task 1
int damageEvaluation(int shipHP, int repairCost)
{
  if (shipHP >= 455) return repairCost;
  if (!isPerfectNum(sumDigit(shipHP))) return repairCost;

  return rnd(repairCost * 1.5);
}

// Task 2
int conflictSimulation(
  char character[FIXED_CHARACTER][MAX_NAME],
  int hp[FIXED_CHARACTER], int skill[FIXED_CHARACTER],
  int shipHP, int repairCost
){
  int luffy = getCharIdx("LUFFY", character), usopp = getCharIdx("USOPP", character);
  int conflictIndex = rnd(skill[luffy] - skill[usopp] + (repairCost + 1000 - 2 * shipHP) / 100.0);

  for(int i = 0; i < 10 && conflictIndex < 255; i++)
  {
    int event = ((conflictIndex % 6) + 6) % 6;
    switch (event) {
      case 0: conflictIndex += 255; break;
      case 1: conflictIndex += 20; break;
      case 2: conflictIndex += 50; break;
      case 3: conflictIndex += 70; break;
      case 4: conflictIndex += 90; break;
      case 5: conflictIndex += 100; break;
      default: break;
    }
  }

  return conflictIndex;
}

// Task 3
void resolveDuel(
  char character[FIXED_CHARACTER][MAX_NAME], int hp[FIXED_CHARACTER], int skill[FIXED_CHARACTER],
  int conflictIndex, int repairCost, char duel[FIXED_CHARACTER][MAX_NAME]
) {
  int luffy = getCharIdx("LUFFY", character);
  int usopp = getCharIdx("USOPP", character);

  const int U = rnd(skill[usopp] + (conflictIndex * 25.0 + repairCost) / 500.0);

  int specialIdx[5], idx = 0;
  for(int i = 0; i < FIXED_CHARACTER; i++)
    if (i != luffy && i != usopp) specialIdx[idx++] = i;

  int minCost = INT_MAX;
  int minMembers = INT_MAX;
  int bestMask = -1;

  for(int mask = 0; mask < 32; ++mask)
  {
    int support = 0, cost = 0, members = 0;

    for(int i = 0; i < 5; ++i)
      if (mask & (1 << i))
      {
        int sIdx = specialIdx[i];
        support += skill[sIdx];
        cost += (hp[sIdx] % 10) + 1;
        members++;
      }

    if (skill[luffy] + support >= U)
    {
      if (cost < minCost)
      {
        minCost = cost;
        minMembers = members;
        bestMask = mask;
      }
      else if (cost == minCost && members < minMembers)
      {
        minMembers = members;
        bestMask = mask;
      }
      else if (cost == minCost && members == minMembers)
        for(int i = 0; i < 5; i++)
        {
          bool hasCurr = mask & (1 << i), hasBest = bestMask & (1 << i);
          if (hasCurr && !hasBest)
          {
            bestMask = mask;
            break;
          }
          else if (!hasCurr && hasBest) break;
        }

    }

  }

  int outLen = 0;
  if (bestMask > -1)
  {
    for(int i = 0; i < 5; ++i)
      if(bestMask & (1 << i)) strcpy(duel[outLen++], character[specialIdx[i]]);
  }

  for(int i = outLen; i < FIXED_CHARACTER; ++i) duel[i][0] = '\0';
}

// Task 4
void decodeCP9Message(
  char character[FIXED_CHARACTER][MAX_NAME],
  int hp[FIXED_CHARACTER], int skill[FIXED_CHARACTER], int conflictIndex,
  int repairCost, char cipherText[], char resultText[]
) {
  resultText[0] = '\0';
  int cipherLen = strlen(cipherText);
  if (cipherLen < 3) return;

  int messageLen = cipherLen - 3;
  char *message = new char[messageLen + 1]; message[messageLen] = '\0';
  memcpy(message, cipherText, messageLen);

  int key = (((conflictIndex + repairCost) % 26) + 26) % 26, B = (key % 5) + 4;
  int XY = cipherText[cipherLen - 1] - '0' + (cipherText[cipherLen - 2] - '0') * 10;

  int sum = 0;
  for(int i = 0; i < messageLen; i++) sum += message[i];
  if ((sum %= 100) != XY)
  {
    delete[] message;
    return;
  }

  for(int i = 0; i < messageLen; i += B)
  {
    int left = i, right = min(i + B, messageLen) - 1;
    while(left < right)
    {
      char temp = message[left];
      message[left] = message[right];
      message[right] = temp;
      left++, right--;
    }
  }

  for(int i = 0; i < messageLen; ++i)
  {
    if (message[i] >= 'A' && message[i] <= 'Z') message[i] = (message[i] - 'A' - key + 26) % 26 + 'A';
    else if (message[i] >= 'a' && message[i] <= 'z') message[i] = (message[i] - 'a' - key + 26) % 26 + 'a';
    else if (message[i] >= '0' && message[i] <= '9') message[i] = (message[i] - '0' - (key % 10) + 10) % 10 + '0';
  }

  strcpy(resultText, message);
  if(strstr(message, "CP9") || strstr(message, "ENIESLOBBY")) strcat(resultText, "_TRUE");
  else strcat(resultText, "_FALSE");

  delete[] message;
}

// Task 5
int analyzeDangerLimit(int grid[MAX_GRID][MAX_GRID], int rows, int cols)
{
  int maxSumRow = 0, maxCell = -1;
  for(int i = 0; i < rows; i++)
  {
    int sumRow = 0;
    for(int j = 0; j < cols; j++)
    {
      sumRow += max(grid[i][j], 0);
      maxCell = max(grid[i][j], maxCell);
    }

    maxSumRow = max(maxSumRow, sumRow);
  }

  return maxSumRow + maxCell;
}

bool evaluateRoute(int grid[MAX_GRID][MAX_GRID], int rows, int cols, int dangerLimit)
{
  if (grid[0][0] == -1 || grid[rows - 1][cols - 1] == -1)
    return false;

  int dp[MAX_GRID][MAX_GRID];

  for(int i = 0; i < rows; i++)
    for(int j = 0; j < cols; j++)
      dp[i][j] = INT_MAX;

  dp[0][0] = grid[0][0];

  for(int i = 0; i < rows; i++)
    for(int j = 0; j < cols; j++)
    {
      if(dp[i][j] == INT_MAX || grid[i][j] == -1) continue;
      if(j + 1 < cols && grid[i][j + 1] > -1)
        dp[i][j + 1] = min(dp[i][j + 1], dp[i][j] + grid[i][j + 1]);
      if(i + 1 < rows && grid[i + 1][j] > -1)
        dp[i + 1][j] = min(dp[i + 1][j], dp[i][j] + grid[i + 1][j]);
    }

  int minDanger = dp[rows - 1][cols - 1];
  return minDanger != INT_MAX && minDanger <= dangerLimit;
}


////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////
