#include <algorithm>
#include <stdexcept>

template <class T>
class ArrayList
{
protected:
  T* data;       // dynamic array to store the list's items
  int capacity;  // size of the dynamic array
  int count;     // number of items stored in the array
public:
  ArrayList()
  {
    capacity = 5;
    count = 0;
    data = new T[5];
  }

  ~ArrayList() { delete[] data; }
  void add(T e);
  void add(int index, T e);
  int size();
  void ensureCapacity(int index);
};

template <class T>
void ArrayList<T>::ensureCapacity(int cap)
{
  /*
      if cap > capacity:
          new_capacity = capacity * 1.5;
          create new array with new_capacity
      else: do nothing
  */
  if (cap > capacity)
  {
    capacity = capacity * 1.5;
    T* newData = new T[capacity];
    for (int i = 0; i < count; i++) newData[i] = data[i];
    delete[] data;
    data = newData;
  }
}

template <class T>
void ArrayList<T>::add(T e)
{
  /* Insert an element into the end of the array. */
  ensureCapacity(count + 1);
  data[count++] = e;
}

template <class T>
void ArrayList<T>::add(int index, T e)
{
  /*
      Insert an element into the array at given index.
      if index is invalid:
          throw std::out_of_range("the input index is out of range!");
  */
  if (index < 0 || index > count) throw std::out_of_range("the input index is out of range!");
  ensureCapacity(count + 1);
  std::move_backward(data + index, data + count, data + count + 1);
  data[index] = e;
  count++;
}

template <class T>
int ArrayList<T>::size()
{
  /* Return the length (size) of the array */
  return count;
}
