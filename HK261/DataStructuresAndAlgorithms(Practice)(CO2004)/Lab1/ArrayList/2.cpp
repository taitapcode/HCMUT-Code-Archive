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
  bool empty();
  void clear();
  T get(int index);
  void set(int index, T e);
  int indexOf(T item);
  bool contains(T item);
  T removeAt(int index);
  bool removeItem(T item);

  void ensureCapacity(int index);
};

template <class T>
T ArrayList<T>::removeAt(int index)
{
  /*
  Remove element at index and return removed value
  if index is invalid:
      throw std::out_of_range("index is out of range");
  */
  if (index < 0 || index >= count) throw std::out_of_range("index is out of range");
  T removedData = data[index];
  std::move(data + index + 1, data + count, data + index);
  --count;

  return removedData;
}

template <class T>
bool ArrayList<T>::removeItem(T item)
{
  /* Remove the first apperance of item in array and return true, otherwise return false */
  for (int i = 0; i < count; i++)
    if (data[i] == item)
    {
      removeAt(i);
      return true;
    }

  return false;
}

template <class T>
void ArrayList<T>::clear()
{
  /*
      Delete array if array is not NULL
      Create new array with: size = 0, capacity = 5
  */

  delete[] data;
  data = new T[5];
  count = 0;
  capacity = 5;
}
