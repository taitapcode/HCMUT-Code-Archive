class Room
{
public:
  double lenght, breadth, height;

  Room(double lenght, double breadth, double height) : lenght(lenght), breadth(breadth), height(height) {};

  double calculateArea()
  {
    return lenght * breadth;
  };

  double calculateVolume()
  {
    return lenght * breadth * height;
  };
};
