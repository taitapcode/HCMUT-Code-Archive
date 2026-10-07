template <class T>
class DLinkedList
{
public:
  class Node;  // forward declaration
protected:
  Node* head;
  Node* tail;
  int count;

public:
  DLinkedList();
  ~DLinkedList();
  void add(const T& e);
  void add(int index, const T& e);
  T removeAt(int index);
  bool removeItem(const T& removeItem);
  bool empty();
  int size();
  void clear();
  T get(int index);
  void set(int index, const T& e);
  int indexOf(const T& item);
  bool contains(const T& item);
};

template <class T>
class Stack
{
protected:
  DLinkedList<T> list;

public:
  Stack() {}
  // void push(T item);
  // T pop();
  // T top();
  // bool empty();
  // int size();
  // void clear();

  void push(T item)
  {
    // TODO: Push new element into the top of the stack
    list.add(item);
  }

  T pop()
  {
    // TODO: Remove an element on top of the stack
    T topValue = top();
    list.removeAt(list.size() - 1);

    return topValue;
  }

  T top()
  {
    // TODO: Get value of the element on top of the stack
    return list.get(list.size() - 1);
  }

  bool empty()
  {
    // TODO: Determine if the stack is empty
    return list.size() == 0;
  }

  int size()
  {
    // TODO: Get the size of the stack
    return list.size();
  }

  void clear()
  {
    // TODO: Clear all elements of the stack
    list.clear();
  }
};
