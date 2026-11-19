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
  SLinkedList() : head(0), tail(0), count(0) {}
  ~SLinkedList() {};
  void add(T e);
  void add(int index, T e);
  int size();
  bool empty();
  T get(int index);
  void set(int index, const T& e);
  int indexOf(const T& item);
  bool contains(const T& item);

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
      next = 0;
    }
    Node(Node* next)
    {
      this->next = next;
    }
    Node(T data, Node* next = 0)
    {
      this->data = data;
      this->next = next;
    }
  };
};

// -------------------------------------------------
template <class T>
T SLinkedList<T>::get(int index)
{
  /* Give the data of the element at given index in the list. */
  if (index < 0 || index >= count) throw std::out_of_range("Index is out of range");
  Node* temp = head;
  for (int i = 0; i < index; i++) temp = temp->next;

  return temp->data;
}

template <class T>
void SLinkedList<T>::set(int index, const T& e)
{
  /* Assign new value for element at given index in the list */
  if (index < 0 || index >= count) throw std::out_of_range("Index is out of range");
  Node* temp = head;
  for (int i = 0; i < index; i++) temp = temp->next;

  temp->data = e;
}

template <class T>
bool SLinkedList<T>::empty()
{
  /* Check if the list is empty or not. */
  return count == 0;
}

template <class T>
int SLinkedList<T>::indexOf(const T& item)
{
  /* Return the first index wheter item appears in list, otherwise return -1 */
  Node* temp = head;
  for (int i = 0; i < count; i++)
  {
    if (temp->data == item) return i;
    temp = temp->next;
  }

  return -1;
}

template <class T>
bool SLinkedList<T>::contains(const T& item)
{
  /* Check if item appears in the list */
  for (Node* temp = head; temp != nullptr; temp = temp->next)
    if (temp->data == item) return true;

  return false;
}
