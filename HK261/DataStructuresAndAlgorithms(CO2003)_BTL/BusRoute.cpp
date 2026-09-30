#include "BusRoute.h"

#include "main.h"

BusRoute::BusRoute(string routeId)
    : routeId(routeId), outboundCount(0), built(false) {}

string BusRoute::getId() const
{
  return routeId;
}

bool BusRoute::isBuilt() const
{
  return built;
}

int BusRoute::getStopCount(Direction direction)
{
  if (!built) return 0;
  if (direction == OUTBOUND) return outboundCount;
  return stops.size() - outboundCount + 2;
}

int BusRoute::physicalIndex(int index, Direction direction)
{
  if (!built || index < 0 || index >= getStopCount(direction)) throw out_of_range("BusRoute::physicalIndex: index out of range");
  if (direction == OUTBOUND) return index;
  return (outboundCount - 1 + index) % stops.size();
}

BusStop& BusRoute::getStop(int index, Direction direction)
{
  if (!built || index < 0 || index >= getStopCount(direction)) throw out_of_range("BusRoute::getStop: index out of range");

  return stops.get(physicalIndex(index, direction));
}

void BusRoute::build(SLinkedList<BusStop>& outbound, SLinkedList<BusStop>& inbound)
{
  stops.clear();
  outboundCount = outbound.size();
  for (int i = 0; i < outbound.size(); i++) stops.add(outbound.get(i));
  for (int i = 1; i < inbound.size() - 1; i++) stops.add(inbound.get(i));

  built = true;
}

int BusRoute::getHopCount(string fromStopId, string toStopId, Direction direction)
{
  if (!built) return -1;

  int count = getStopCount(direction), fromIdx = -1, toIdx = -1;
  for (int i = 0; i < count; i++)
  {
    if (getStop(i, direction).getId() == fromStopId) fromIdx = i;
    if (getStop(i, direction).getId() == toStopId) toIdx = i;
  }
  if (fromIdx == -1 || toIdx == -1 || toIdx < fromIdx) return -1;
  return toIdx - fromIdx;
}
