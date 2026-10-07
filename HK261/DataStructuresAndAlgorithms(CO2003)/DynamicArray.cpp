#include <algorithm>
#include <stdexcept>

template <typename T>
class DynamicArray
{
private:
  const static int INITIAL_CAPACITY = 10;
  T* array;
  int capacity;
  int count;

  void ensureCapacity()
  {
    if (count < capacity) return;

    capacity = capacity == 0 ? INITIAL_CAPACITY : capacity * 3 / 2;
    T* newArray = new T[capacity];
    for (int i = 0; i < count; i++) newArray[i] = array[i];

    delete[] array;
    array = newArray;
  }

public:
  DynamicArray() : capacity(INITIAL_CAPACITY), count(0), array(new T[INITIAL_CAPACITY]) {}
  DynamicArray(int capacity) : capacity(capacity), count(0), array(new T[capacity]) {}
  ~DynamicArray() { delete[] array; }

  void add(const T& element)
  {
    ensureCapacity();
    array[count++] = element;
  }

  void add(int index, const T& element)
  {
    if (index < 0 || index > count) throw std::out_of_range("Index out of range");
    ensureCapacity();

    std::move_backward(array + index, array + count, array + count + 1);
    array[index] = element;
    count++;
  }

  T removeAt(int index)
  {
    if (index < 0 || index >= count) throw std::out_of_range("Index out of range");
    T removedElement = array[index];
    std::move(array + index + 1, array + count, array + index);
    count--;
    return removedElement;
  }

  bool removeItem(const T& element)
  {
    int index = indexOf(element);
    if (index == -1) return false;
    removeAt(index);
    return true;
  }

  void clear()
  {
    delete[] array;
    array = new T[INITIAL_CAPACITY];
    capacity = INITIAL_CAPACITY;
    count = 0;
  }

  T get(int index) const
  {
    if (index < 0 || index >= count) throw std::out_of_range("Index out of range");
    return array[index];
  }

  void set(int index, T element)
  {
    if (index < 0 || index >= count) throw std::out_of_range("Index out of range");
    array[index] = element;
  }

  bool empty() const
  {
    return count == 0;
  }

  int size() const
  {
    return count;
  }

  int indexOf(const T& element) const
  {
    for (int i = 0; i < count; i++)
      if (element == array[i]) return i;

    return -1;
  }
};
