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

public:
  class Node
  {
  private:
    T data;
    Node* next;
    Node* previous;
    friend class DLinkedList<T>;

  public:
    Node()
    {
    }

    Node(const T& data)
    {
    }
  };
};

template <class T>
void DLinkedList<T>::add(const T& e)
{
  /* Insert an element into the end of the list. */
  Node* newNode = new Node(e);
  if (count == 0)
    head = tail = newNode;
  else
  {
    newNode->previous = tail;
    tail = tail->next = newNode;
  }
  count++;
}

template <class T>
void DLinkedList<T>::add(int index, const T& e)
{
  if (index < 0 || index > count) return;
  if (index == count) return add(e);

  Node* newNode = new Node(e);
  if (index == 0)
  {
    newNode->next = head;
    head->previous = newNode;
    head = newNode;
  }
  else
  {
    Node* prev = head;
    for (int i = 0; i < index - 1; i++) prev = prev->next;

    newNode->next = prev->next;
    newNode->previous = prev;
    prev->next = prev->next->previous = newNode;
  }

  count++;
}

template <class T>
int DLinkedList<T>::size()
{
  /* Return the length (size) of list */
  return count;
}
