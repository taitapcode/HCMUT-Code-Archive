template <class T>
class DLinkedList
{
public:
  class Node;  // Forward declaration
protected:
  Node* head;
  Node* tail;
  int count;

public:
  DLinkedList();
  ~DLinkedList();
  void add(const T& e);
  void add(int index, const T& e);
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
    Node* previous;
    friend class DLinkedList<T>;

  public:
    Node();

    Node(const T& data);
  };
};
template <class T>
T DLinkedList<T>::get(int index)
{
  /* Give the data of the element at given index in the list. */
  Node* curr = head;
  for (int i = 0; i < index; i++) curr = curr->next;

  return curr->data;
}

template <class T>
void DLinkedList<T>::set(int index, const T& e)
{
  /* Assign new value for element at given index in the list */
  Node* curr = head;
  for (int i = 0; i < index; i++) curr = curr->next;

  curr->data = e;
}

template <class T>
bool DLinkedList<T>::empty()
{
  /* Check if the list is empty or not. */
  return count == 0;
}

template <class T>
int DLinkedList<T>::indexOf(const T& item)
{
  /* Return the first index wheter item appears in list, otherwise return -1 */
  Node* curr = head;
  for (int i = 0; i < count; i++, curr = curr->next)
    if (curr->data == item) return i;

  return -1;
}

template <class T>
bool DLinkedList<T>::contains(const T& item)
{
  /* Check if item appears in the list */
  return indexOf(item) > -1;
}
