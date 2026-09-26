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
  void add(const T& e);
  void add(int index, const T& e);
  int size();

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
    Node(T data, Node* next)
    {
      this->data = data;
      this->next = next;
    }
  };
};

// -------------------------------------------------------

template <class T>
void SLinkedList<T>::add(const T& e)
{
  add(count, e);
}

template <class T>
void SLinkedList<T>::add(int index, const T& e)
{
  if (!head)
    head = tail = new Node(e, 0);
  else
  {
    Node** p = &head;
    for (int i = 0; i < index; i++) p = &((*p)->next);
    *p = new Node(e, *p);
    if (index == count) tail = *p;
  }
  count++;
}

template <class T>
int SLinkedList<T>::size()
{
  return count;
}
