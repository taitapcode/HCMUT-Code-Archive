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
  T removeAt(int index);
  bool removeItem(const T& item);
  void clear();

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
T DLinkedList<T>::removeAt(int index)
{
  /* Remove element at index and return removed value */
  Node* deleteNode = nullptr;
  if (count == 1)
  {
    deleteNode = head;
    head = tail = nullptr;
  }
  else if (index == 0)
  {
    deleteNode = head;
    head = head->next;
    head->previous = nullptr;
  }
  else if (index == count - 1)
  {
    deleteNode = tail;
    tail = tail->previous;
    tail->next = nullptr;
  }
  else
  {
    deleteNode = head;
    for (int i = 0; i < index; i++) deleteNode = deleteNode->next;
    deleteNode->previous->next = deleteNode->next;
    deleteNode->next->previous = deleteNode->previous;
  }

  count--;
  T deleteData = deleteNode->data;
  delete deleteNode;
  return deleteData;
}

template <class T>
bool DLinkedList<T>::removeItem(const T& item)
{
  /* Remove the first appearance of item in list and return true, otherwise return false */
  int index = indexOf(item);
  if (index == -1) return false;
  removeAt(index);
  return true;
}

template <class T>
void DLinkedList<T>::clear()
{
  /* Remove all elements in list */
  Node* curr = head;
  while (curr != nullptr)
  {
    Node* nextNode = curr->next;
    delete curr;
    curr = nextNode;
  }
  head = tail = nullptr;
  count = 0;
}
