#ifndef BKMOVE_H
#define BKMOVE_H

#include <vector>

#include "BusRoute.h"
#include "QuickSort.h"
#include "main.h"
using namespace std;

class BKMove
{
private:
  vector<BusRoute*> routes;

public:
  BKMove() = default;

  void addRoute(BusRoute* route);
  int getRouteCount() const;
  BusRoute* getRoute(int index);

  vector<RouteResult> findDirectRoutes(string fromStopId, string toStopId);
  vector<JourneyResult> findJourneys(string fromStopId, string toStopId);
};

#endif
