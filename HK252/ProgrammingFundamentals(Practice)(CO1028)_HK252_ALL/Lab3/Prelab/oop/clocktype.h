class ClockType
{
public:
  ClockType(int, int, int);
  ClockType();
  void printTime() const;
  void setTime(int, int, int);
  void getTime(int&, int&, int&) const;


private:
  int hr;
  int min;
  int sec;
};

