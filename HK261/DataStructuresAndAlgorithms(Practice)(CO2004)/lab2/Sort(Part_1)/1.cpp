#include <iostream>
#include <sstream>
using namespace std;

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
  SLinkedList()
  {
    this->head = nullptr;
    this->tail = nullptr;
    this->count = 0;
  }
  ~SLinkedList() {};
  void add(T e)
  {
    Node* pNew = new Node(e);

    if (this->count == 0)
    {
      this->head = this->tail = pNew;
    }
    else
    {
      this->tail->next = pNew;
      this->tail = pNew;
    }

    this->count++;
  }
  int size()
  {
    return this->count;
  }
  void printList()
  {
    stringstream ss;
    ss << "[";
    Node* ptr = head;
    while (ptr != tail)
    {
      ss << ptr->data << ",";
      ptr = ptr->next;
    }

    if (count > 0)
      ss << ptr->data << "]";
    else
      ss << "]";
    cout << ss.str() << endl;
  }

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
    Node(T data)
    {
      this->data = data;
      this->next = nullptr;
    }
  };

  void bubbleSort();
};

// ---------------------
template <class T>
void SLinkedList<T>::bubbleSort()
{
  if (count < 2) return;
  for (int i = 0; i < count - 1; i++)
  {
    bool isSwapped = false;
    Node* ptr = head;
    for (int j = 0; j < count - 1 - i; j++, ptr = ptr->next)
      if (ptr->data > ptr->next->data)
      {
        T temp = ptr->data;
        ptr->data = ptr->next->data;
        ptr->next->data = temp;
        isSwapped = true;
      }
    if (!isSwapped) break;
    printList();
  }
}
