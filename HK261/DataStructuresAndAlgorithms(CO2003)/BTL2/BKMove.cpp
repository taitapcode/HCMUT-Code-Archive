#include "BKMove.h"

// Q3 + Q4: implement the public methods below.
// BKMove.h intentionally leaves the private section unspecified: design your
// own member variables and helpers for registered stops and pending arrivals.
// You may add classes and extend BST/AVL/Heap within the permitted files.
// Do not change the public API or the supplied StopInfo/ArrivalEvent fields.
// Do not use global storage shared by unrelated BKMove objects.

BKMove::BKMove() = default;
BKMove::~BKMove()
{
  byEvent.clear(&BKMove::disposeEvent);
  byKey.clear(&BKMove::disposeStop);
};

bool BKMove::nextPrefix(const std::string& prefix, std::string& upper)
{
  upper = prefix;
  while (!upper.empty() && (unsigned char)upper.back() == 0xFF)
    upper.pop_back();
  if (upper.empty()) return false;
  upper.back() = (char)((unsigned char)upper.back() + 1);
  return true;
}

// Q3 -- Bus Stop Finder
bool BKMove::addStop(const StopInfo& stop)
{
  if (stop.stopId.empty() || stop.displayName.empty() ||
      stop.searchKey.empty())
    return false;
  if (byId.find(stop.stopId)) return false;

  StopNode* newStop = new StopNode(stop);
  byId.insert(stop.stopId, newStop);
  byKey.insert(stop.searchKey + '\0' + stop.stopId, newStop);
  return true;
}

bool BKMove::removeStop(const std::string& stopId)
{
  StopNode* node = byId.find(stopId);
  if (!node || node->pending > 0) return false;

  byKey.erase(node->info.searchKey + '\0' + stopId);
  byId.erase(stopId);
  delete node;
  return true;
}

bool BKMove::findStop(const std::string& stopId, StopInfo& out) const
{
  StopNode* node = byId.find(stopId);
  if (!node) return false;
  out = node->info;
  return true;
}

int BKMove::getStopCount() const
{
  return byId.size();
}

std::vector<StopInfo> BKMove::findStops(const std::string& prefix, int page, int pageSize) const
{
  std::vector<StopInfo> rows;
  if (page < 0 || pageSize <= 0) return rows;

  const int begin = byKey.rank(prefix);
  int end = byKey.size();
  std::string upper;
  if (nextPrefix(prefix, upper)) end = byKey.rank(upper);

  long long start = (long long)begin + (long long)page * pageSize;
  if (start >= end) return rows;

  int limit = (int)(end - start);
  if (limit > pageSize) limit = pageSize;

  std::vector<StopNode*> picked;
  byKey.collect((int)start, limit, picked);

  rows.reserve(picked.size());
  for (size_t i = 0; i < picked.size(); ++i) rows.push_back(picked[i]->info);
  return rows;
}

// Q4 -- Live Arrival Board
bool BKMove::scheduleArrival(const ArrivalEvent& event)
{
  if (event.eventId.empty() || event.stopId.empty() || event.routeId.empty())
    return false;
  if (event.arrivalTime < 0) return false;
  if (byEvent.find(event.eventId)) return false;

  StopNode* stop = byId.find(event.stopId);
  if (!stop) return false;

  EventSlot* slot = new EventSlot(event, stop);
  byEvent.insert(event.eventId, slot);
  slot->pos = (int)stop->board.size();
  stop->board.push_back(slot);
  ++stop->pending;
  reheapUp(stop, slot->pos);
  return true;
}

bool BKMove::peekNextArrival(const std::string& stopId, ArrivalEvent& out) const
{
  StopNode* stop = byId.find(stopId);
  if (!stop || stop->board.empty()) return false;
  out = stop->board.front()->event;
  return true;
}

bool BKMove::processNextArrival(
    const std::string& stopId, ArrivalEvent& out)
{
  StopNode* stop = byId.find(stopId);
  if (!stop || stop->board.empty()) return false;

  EventSlot* slot = stop->board.front();
  out = slot->event;
  byEvent.erase(slot->event.eventId);
  detachSlot(stop, 0);
  delete slot;
  return true;
}

bool BKMove::rescheduleArrival(const std::string& eventId, int newArrivalTime)
{
  if (newArrivalTime < 0) return false;

  EventSlot* slot = byEvent.find(eventId);
  if (!slot) return false;

  slot->event.arrivalTime = newArrivalTime;
  reheapUp(slot->owner, slot->pos);
  reheapDown(slot->owner, slot->pos);
  return true;
}

bool BKMove::cancelArrival(const std::string& eventId)
{
  EventSlot* slot = byEvent.find(eventId);
  if (!slot) return false;

  StopNode* stop = slot->owner;
  byEvent.erase(eventId);
  detachSlot(stop, slot->pos);
  delete slot;
  return true;
}
