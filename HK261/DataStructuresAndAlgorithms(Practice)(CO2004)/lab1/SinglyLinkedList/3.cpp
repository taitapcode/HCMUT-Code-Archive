#include <stdexcept>

template <class T>
class SLinkedList
{
public:
  class Node;  // Forward declaration
protected:
  Node* head;
  Node* tail;
  int count;

public:
  SLinkedList();
  ~SLinkedList();
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
  bool removeItem(const T& item);

public:
  class Node
  {
  private:
    T data;
    Node* next;
    friend class SLinkedList<T>;

  public:
    Node()
    {
    }
    Node(Node* next)
    {
    }
    Node(T data, Node* next = nullptr)
    {
    }
  };
};

// -------------------------------------------------
template <class T>
T SLinkedList<T>::removeAt(int index)
{
  if (index < 0 || index >= count) throw std::out_of_range("Index is out of range");

  Node* deletedNode = nullptr;
  T deletedData;

  if (index == 0)
  {
    deletedNode = head;
    head = head->next;
    deletedData = deletedNode->data;

    if (count == 1) tail = nullptr;
  }
  else
  {
    Node* prev = head;
    for (int i = 0; i < index - 1; i++) prev = prev->next;

    deletedNode = prev->next;
    deletedData = deletedNode->data;
    prev->next = deletedNode->next;

    if (index == count - 1) tail = prev;
  }

  delete deletedNode;
  count--;
  return deletedData;
}

template <class T>
bool SLinkedList<T>::removeItem(const T& item)
{
  Node* temp = head;

  for (int i = 0; i < count; i++, temp = temp->next)
    if (item == temp->data)
    {
      removeAt(i);
      return true;
    }

  return false;
}

template <class T>
void SLinkedList<T>::clear()
{
  while (head != nullptr)
  {
    Node* nextNode = head->next;
    delete head;
    head = nextNode;
  }

  tail = nullptr;
  count = 0;
}
