#include <iostream>
#include <string>
using namespace std;

template <typename T>
class SLinkedList
{
public:
  virtual void add(T e) = 0;
  virtual void add(int index, T e) = 0;
  virtual T removeAt(int index) = 0;
  virtual bool removeItem(T item, void (*removeItemData)(T) = 0) = 0;
  virtual bool empty() = 0;
  virtual int size() = 0;
  virtual void clear() = 0;
  virtual T& get(int index) = 0;
  virtual int indexOf(T item) = 0;
  virtual bool contains(T item) = 0;
  virtual string toString(string (*item2str)(T&) = 0) = 0;
};

class Polynomial;
class Term
{
private:
  double coeff;
  int exp;
  friend class Polynomial;

public:
  Term(double coeff = 0.0, int exp = 0);
  bool operator==(const Term& rhs) const;
  friend ostream& operator<<(ostream& os, const Term& term);
};
class Polynomial
{
private:
  SLinkedList<Term>* terms;

public:
  Polynomial();
  ~Polynomial();
  void insertTerm(const Term& term);
  void insertTerm(double coeff, int exp);
  void print();
};

void Polynomial::insertTerm(const Term& term)
{
  insertTerm(term.coeff, term.exp);
}

void Polynomial::insertTerm(double coeff, int exp)
{
  if (coeff == 0) return;

  for (int i = 0; i < terms->size(); i++)
  {
    Term curr = terms->get(i);
    if (curr.exp == exp)
    {
      curr.coeff += coeff;
      terms->removeAt(i);
      if (curr.coeff != 0)
      {
        terms->add(i, curr);
      }
      return;
    }
    else if (curr.exp < exp)
    {
      terms->add(i, Term(coeff, exp));
      return;
    }
  }

  terms->add(Term(coeff, exp));
}
