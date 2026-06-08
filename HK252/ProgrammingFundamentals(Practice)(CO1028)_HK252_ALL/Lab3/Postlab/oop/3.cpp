#include <iostream>
using namespace std;

template <class T>
class Array2D {
private:
	int r;
	int c;
	T** arr;

public:
  Array2D(int r, int c) : r(r), c(c)
  {
    this->arr = new T*[r];
    this->arr[0] = new T[r * c];
    for(int i = 1; i < r; i++)
      this->arr[i] = this->arr[0] + (i * c);
  }

  Array2D(int r, int c, T init) : Array2D(r, c)
  {
    for(int i = 0; i < (r * c); i++) this->arr[0][i] = init;
  }

  ~Array2D()
  {
    if (arr)
    {
      delete[] arr[0];
      delete[] arr;
    }
  }


  Array2D<T>* operator* (const Array2D<T> &other)
  {
    if (this->c != other.r) return NULL;
    Array2D<T> *res = new Array2D<T>(this->r, other.c, 0);

    for(int i = 0; i < this->r; i++)
      for(int j = 0; j < other.c; j++)
        for(int k = 0; k < this->c; k++)
          res->arr[i][j] += this->arr[i][k] * other.arr[k][j];

    return res;
  }

  T* operator[] (const int idx) { return this->arr[idx]; }

  int getR() {return this->r;}
  int getC() {return this->c;}

  void disp() {
    for (int i = 0; i < this->r; i++) {
      for (int j = 0; j < this->c; j++) {
        cout << arr[i][j] << " ";
      }
      cout << endl;
    }
  }
};
