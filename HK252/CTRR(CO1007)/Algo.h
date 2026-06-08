#include <algorithm>
#include <cmath>

#include "PathNode.h"

// Task 1
PathNode* findSocialPath(double adjMatrix[100][100], int startPerson, int goalPerson);

// Task 2
PathNode* findDronePath(double weightMatrix[100][100], int coords[100][2],
                        int startPoint, int goalPoint, int mode);

// Task 3
PathNode* findWarehousePath(int warehouse[100][100], int m, int n, int startX,
                            int startY, int goalX, int goalY, int mode);

// Task 4
PathNode* findEvacuationPath(int floorPlan[100][100], int m, int n, int startX, int startY,
                             int exitX, int exitY, double weightMatrix[100][100], int mode);
