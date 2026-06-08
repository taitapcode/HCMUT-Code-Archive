class Integer
{
private:
  int val;

public:
  Integer(int x) : val(x) {};
  void setValue(int x) { val = x; }
  int getValue() { return val; }
};
