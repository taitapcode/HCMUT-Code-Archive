#include "BKMove.h"

#include "BusRoute.h"
#include "main.h"

void BKMove::addRoute(BusRoute* route)
{
  if (route == nullptr) throw invalid_argument("route must not be null");
  routes.push_back(route);
}

int BKMove::getRouteCount() const
{
  return static_cast<int>(routes.size());
}

BusRoute* BKMove::getRoute(int index)
{
  if (index < 0 || index >= static_cast<int>(routes.size())) throw out_of_range("route index is out of range");
  return routes[index];
}

static int compareRouteResult(RouteResult& a, RouteResult& b)
{
  if (a.hopCount != b.hopCount) return a.hopCount < b.hopCount ? -1 : 1;
  if (a.routeId != b.routeId) return a.routeId < b.routeId ? -1 : 1;
  if (a.direction != b.direction) return a.direction < b.direction ? -1 : 1;

  return 0;
}

vector<RouteResult> BKMove::findDirectRoutes(string fromStopId, string toStopId)
{
  vector<RouteResult> results;

  for (BusRoute* route : routes)
  {
    if (!route || !route->isBuilt()) continue;

    for (Direction direction : {OUTBOUND, INBOUND})
    {
      int hops = route->getHopCount(fromStopId, toStopId, direction);
      if (hops >= 0) results.push_back(RouteResult(route->getId(), direction, hops));
    }
  }

  if (results.size() > 1)
  {
    QuickSort<RouteResult> sorter;
    sorter.sort(results.data(), static_cast<int>(results.size()), compareRouteResult);
  }

  return results;
}

static int compareJourneyResult(JourneyResult& a, JourneyResult& b)
{
  if (a.transfers != b.transfers) return a.transfers < b.transfers ? -1 : 1;
  if (a.totalHops != b.totalHops) return a.totalHops < b.totalHops ? -1 : 1;
  if (a.firstRouteId != b.firstRouteId) return a.firstRouteId < b.firstRouteId ? -1 : 1;
  if (a.firstDirection != b.firstDirection) return a.firstDirection < b.firstDirection ? -1 : 1;
  if (a.transferStopId != b.transferStopId) return a.transferStopId < b.transferStopId ? -1 : 1;
  if (a.secondRouteId != b.secondRouteId) return a.secondRouteId < b.secondRouteId ? -1 : 1;
  if (a.secondDirection != b.secondDirection) return a.secondDirection < b.secondDirection ? -1 : 1;
  return 0;
}

vector<JourneyResult> BKMove::findJourneys(string fromStopId, string toStopId)
{
  vector<JourneyResult> journeys;
  vector<RouteResult> directRoutes = findDirectRoutes(fromStopId, toStopId);
  for (auto& route : directRoutes)
  {
    journeys.push_back(JourneyResult(
        route.routeId,
        route.direction,
        route.hopCount));
  }

  for (BusRoute* route1 : routes)
  {
    if (!route1 || !route1->isBuilt()) continue;

    for (Direction d1 : {OUTBOUND, INBOUND})
    {
      int stopCount = route1->getStopCount(d1), fromIdx = -1;
      for (int i = 0; i < stopCount; i++)
        if (route1->getStop(i, d1).getId() == fromStopId)
        {
          fromIdx = i;
          break;
        }

      if (fromIdx == -1) continue;

      for (int i = fromIdx + 1; i < stopCount; i++)
      {
        std::string transferStopId = route1->getStop(i, d1).getId();
        if (transferStopId == fromStopId || transferStopId == toStopId) continue;

        int hops1 = i - fromIdx;

        for (BusRoute* route2 : routes)
        {
          if (!route2 || !route2->isBuilt() || route2->getId() == route1->getId()) continue;

          for (Direction d2 : {OUTBOUND, INBOUND})
          {
            int hops2 = route2->getHopCount(transferStopId, toStopId, d2);
            if (hops2 > 0) journeys.push_back(JourneyResult(
                route1->getId(), d1, transferStopId,
                route2->getId(), d2, hops1 + hops2));
          }
        }
      }
    }
  }

  if (journeys.size() > 1)
  {
    QuickSort<JourneyResult> sorter;
    sorter.sort(journeys.data(), static_cast<int>(journeys.size()), compareJourneyResult);
  }

  return journeys;
}
