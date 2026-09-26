#include <bits/stdc++.h>

class Point
{
private:
  double x, y;

public:
  Point() : x(0), y(0) {}

  Point(double x, double y) : x(x), y(y) {}

  void setX(double x)
  {
    this->x = x;
  }

  void setY(double y)
  {
    this->y = y;
  }

  double getX() const
  {
    return x;
  }

  double getY() const
  {
    return y;
  }

  double distanceToPoint(const Point& pointA)
  {
    double dX = pointA.getX() - x, dY = pointA.getY() - y;
    return sqrt(dX * dX + dY * dY);
  }
};

class Circle
{
private:
  Point center;
  double radius;

public:
  Circle() : center{0, 0}, radius(0) {}

  Circle(Point center, double radius) : center(center), radius(radius) {}

  Circle(const Circle& circle) : center(circle.center), radius(circle.radius) {}

  void setCenter(Point point)
  {
    this->center = point;
  }

  void setRadius(double radius)
  {
    this->radius = radius;
  }

  Point getCenter() const
  {
    return center;
  }

  double getRadius() const
  {
    return radius;
  }

  void printCircle()
  {
    printf("Center: {%.2f, %.2f} and Radius %.2f\n", this->center.getX(), this->center.getY(), this->radius);
  }
};
