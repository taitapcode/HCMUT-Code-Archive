#include <iostream>
#include <cstring>
using namespace std;

class Book
{
  char* name;
public:
  Book(const char*);
  ~Book();

  void display();
  char* getName();
};

Book::Book(const char* name)
{
  int len = strlen(name);
  this->name = new char[len + 1];
  strcpy(this->name, name);
  this->name[len] = '\0';
}

Book::~Book()
{
  delete[] this->name;
}

void Book::display()
{
  cout << "Book: " << this->name << endl;
}

char* Book::getName()
{
  return this->name;
}
