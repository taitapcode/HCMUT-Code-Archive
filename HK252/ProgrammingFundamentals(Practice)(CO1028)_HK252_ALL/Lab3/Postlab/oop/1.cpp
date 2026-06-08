template <typename T>
class Array {
public:
  Array(int size, T initValue);
  ~Array();

  void setAt(int idx, const T & value);
  T getAt(int idx);

  void print();

private:
  int size;
  T * p;
};


template<typename T>
void Array<T>::setAt(int idx, const T &value)
{
  if (idx < 0 || idx >= this->size) throw -1;
  this->p[idx] = value;
}

template<typename T>
T Array<T>::getAt(int idx)
{
  if (idx < 0 || idx >= this->size) throw -1;
  return p[idx];
}
