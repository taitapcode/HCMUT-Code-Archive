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

  Node dummy(-1, head);
  Node* temp = dummy.next;
  for (int i = 0; i < index - 1; i++) temp = temp->next;

  Node* deleteNode = temp->next;
  T data = deleteNode->data;

  temp->next = deleteNode->next;
  delete deleteNode;

  return data;
}

template <class T>
bool SLinkedList<T>::removeItem(const T& item)
{
  Node* temp = head;
  while (temp != nullptr)
  {
    if (temp->data == item)
    {
      removeAt(indexOf(item));
      return true;
    }
    temp = temp->next;
  }
}

template <class T>
void SLinkedList<T>::clear()
{
}
