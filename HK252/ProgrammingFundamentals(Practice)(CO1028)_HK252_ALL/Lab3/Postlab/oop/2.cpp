#include <math.h>
class Coordinate
{
public:
  Coordinate(double x = 0, double y = 0) : x(x), y(y) {};
  void setX(double x){ this->x = x; }
  void setY(double y) { this->y = y; }
  double getX() { return this->x; }
  double getY() { return this->y; }

  double distanceTo(Coordinate &other)
  {
    return abs(this->x - other.x) + abs(this->y - other.y);
  }

private:
  double x, y;
};
