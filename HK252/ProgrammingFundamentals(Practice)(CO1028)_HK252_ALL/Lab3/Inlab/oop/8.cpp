#include <iostream>
using namespace std;

class ClockType
{
   public:
      void setTime(int, int, int);
      void printTime() const;
      void incrementSeconds();
      void incrementMinutes();
      void incrementHours();
      ClockType(int, int, int);
      ClockType();

   private:
      int hr;
      int min;
      int sec;
};

void ClockType::printTime() const
{
   if (hr < 10)
      cout << "0";
   cout << hr << ":";
   if (min < 10)
      cout << "0";
   cout << min << ":";
   if (sec < 10)
      cout << "0";
   cout << sec;
}

void ClockType::incrementSeconds()
{
  if (++sec >= 60) min += sec / 60, sec %= 60;
  if (min >= 60) hr += min /60, min %= 60;
  hr %= 24;
}

void ClockType::incrementMinutes()
{
  if (++min >= 60) hr += min /60, min %= 60;
  hr %= 24;
}

void ClockType::incrementHours()
{
  ++hr %= 24;
}
