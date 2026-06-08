/*
* Ho Chi Minh City University of Technology
* Faculty of Computer Science and Engineering
* Initial code for Assignment 1
* Programming Fundamentals Spring 2026
* Date: 27.01.2026
*/

//The library here is concretely set, students are not allowed to include any other libraries.

#include "water_seven.h"
#define el '\n'

using namespace std;


char character[FIXED_CHARACTER][MAX_NAME];
int hp[FIXED_CHARACTER], skill[FIXED_CHARACTER], shipHP, repairCost;

int main(int argc, const char * argv[])
{
  cout << "Task 0" << el;
  if(!readInput("opw_tc_01_input", character, hp, skill, shipHP, repairCost))
  {
    cout << "Not gud";
    return 1;
  }

  for(int i = 0; i < FIXED_CHARACTER; i++)
    cout << character[i] << ' ' << hp[i] << ' ' << skill[i] << el;
  cout << shipHP << ' ' << repairCost << el << el;

  cout << "Task 1" << el;
  cout << damageEvaluation(411, 253) << el << el;

  cout << "Task 2" << el;
  cout << conflictSimulation(character, hp, skill, shipHP, repairCost) << el << el;

  cout << "Task 3" << el;
  char duel[FIXED_CHARACTER][MAX_NAME] = { '\0', '\0', '\0', '\0', '\0' };
  resolveDuel(character, hp, skill, 120, 1500, duel);
  for(int i = 0; duel[i][0] != 0; i++) cout << duel[i] << ' ';
  cout << el << el;

  cout << "Task 4" << el;
  char res[100] = {}, cipher[] = "AQ 7XK#96";
  decodeCP9Message(character, hp, skill, 120, 1500, cipher, res);
  cout << res << el;

  cout << "Task 5.1" << el;
  int grid[MAX_GRID][MAX_GRID] = {
    {1, -1, 0},
    {0, 2, 3},
    {-1, 1, 2},
  };
  cout << analyzeDangerLimit(grid, 3, 3) << el << el;

  cout << "Task 5.2" << el;
  cout << evaluateRoute(grid, 3, 3, 5) << el;
  cout << evaluateRoute(grid, 3, 3, 4) << el;
}
