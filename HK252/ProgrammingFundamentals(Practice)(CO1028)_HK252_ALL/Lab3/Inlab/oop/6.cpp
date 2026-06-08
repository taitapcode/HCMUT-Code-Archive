#include<iostream>
using namespace std;

class Integer
{
private:
  int val;
public:
  Integer(int);
  Integer(Integer*);

  Integer operator+(const Integer&) const;
  Integer operator+(const int&) const;
  void print();
};

Integer::Integer(int val) : val(val) {}
Integer::Integer(Integer* other) : val(other->val) {}

void Integer::print() {
  cout << this->val << endl;
}

Integer Integer::operator+(const Integer& other) const
{
  return Integer(this->val + other.val);
}

Integer Integer::operator+(const int& other) const
{
  return Integer(this->val + other);
}

int main() {
  int x, y, z;
  cin >> x >> y >> z;
  Integer a(x);
  Integer b(y);
  Integer* t = new Integer(z);
  Integer c(t);

  a.print(); b.print();
  (a + b + c + 4).print();

  delete t;
  return 0;
}
