#include <cmath>
#include <iostream>

#include "Algo.h"

using namespace std;

void printPath(PathNode* head)
{
  cout << "Solution Path:\n";
  while (head != nullptr)
  {
    cout << "Node: " << head->name
         << " | f: " << head->f
         << " | g: " << head->g
         << " | h: " << head->h << "\n";
    head = head->next;
  }
  cout << "--------------------------------------------------\n";
}

void freePath(PathNode* head)
{
  while (head != nullptr)
  {
    PathNode* temp = head;
    head = head->next;
    delete temp;
  }
}

int main()
{
  // -------------------------------------------------------------------------
  // TASK 1: Degrees of Separation in a Social Network
  // -------------------------------------------------------------------------
  cout << "[RUNNING] Task 1: Degrees of Separation...\n";
  double adjMatrixT1[100][100] = {0};
  adjMatrixT1[0][1] = 1;
  adjMatrixT1[0][2] = 1;
  adjMatrixT1[1][0] = 1;
  adjMatrixT1[1][3] = 1;
  adjMatrixT1[2][0] = 1;
  adjMatrixT1[2][3] = 1;
  adjMatrixT1[3][1] = 1;
  adjMatrixT1[3][2] = 1;
  adjMatrixT1[3][4] = 1;
  adjMatrixT1[4][3] = 1;

  PathNode* pathT1 = findSocialPath(adjMatrixT1, 0, 4);
  printPath(pathT1);
  freePath(pathT1);

  // -------------------------------------------------------------------------
  // TASK 2: Drone Delivery in 2D Space
  // -------------------------------------------------------------------------
  cout << "[RUNNING] Task 2: Drone Delivery (Testing Mode 2 - Euclidean)...\n";
  double weightMatrixT2[100][100] = {0};
  int coordsT2[100][2] = {0};

  coordsT2[0][0] = 0;
  coordsT2[0][1] = 0;
  coordsT2[1][0] = 3;
  coordsT2[1][1] = 1;
  coordsT2[2][0] = 3;
  coordsT2[2][1] = 4;

  weightMatrixT2[0][1] = sqrt(10);
  weightMatrixT2[1][0] = sqrt(10);
  weightMatrixT2[1][2] = 3.0;
  weightMatrixT2[2][1] = 3.0;
  weightMatrixT2[0][2] = 5.0;
  weightMatrixT2[2][0] = 5.0;

  PathNode* pathT2 = findDronePath(weightMatrixT2, coordsT2, 0, 2, 2);
  printPath(pathT2);
  freePath(pathT2);

  // -------------------------------------------------------------------------
  // TASK 3: Warehouse Robot Navigation with Obstacles
  // -------------------------------------------------------------------------
  cout << "[RUNNING] Task 3: Warehouse Robot (Testing Mode 2 - Chebyshev)...\n";
  int warehouseT3[100][100] = {0};
  int m3 = 3, n3 = 3;

  PathNode* pathT3 = findWarehousePath(warehouseT3, m3, n3, 0, 0, 2, 2, 2);  // Mode 2
  printPath(pathT3);
  freePath(pathT3);

  // -------------------------------------------------------------------------
  // TASK 4: Evacuation Route Planning
  // -------------------------------------------------------------------------
  cout << "[RUNNING] Task 4: Evacuation Route Planning...\n";
  int floorPlanT4[100][100] = {0};
  double weightMatrixT4[100][100] = {0};
  int m4 = 2, n4 = 2;

  PathNode* pathT4 = findEvacuationPath(floorPlanT4, m4, n4, 0, 0, 1, 1, weightMatrixT4, 2);
  printPath(pathT4);

  cout << "Goc ma tran trong so weightMatrix T4 (4x4):\n";
  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++) cout << weightMatrixT4[i][j] << "\t";
    cout << "\n";
  }
  cout << "--------------------------------------------------\n";
  freePath(pathT4);

  cout << "\n[SUCCESS] Chay thu nghiem tat ca cac Task hoan tat!\n";
  return 0;
}
