class LinkedList
{
public:
  class Node;

private:
  Node* head;
  Node* tail;
  int size;

public:
  class Node
  {
  private:
    int value;
    Node* next;
    friend class LinkedList;

  public:
    Node()
    {
      this->next = nullptr;
    }
    Node(Node* node)
    {
      this->value = node->value;
      this->next = node->next;
    }
    Node(int value, Node* next = nullptr)
    {
      this->value = value;
      this->next = next;
    }
  };
  LinkedList() : head(nullptr), tail(nullptr), size(0) {};
  void partition(int k);
};

void LinkedList::partition(int k)
{
  if (!head || !head->next) return;

  Node dummyLess(0), dummyEqual(0), dummyGreater(0);
  Node *tailLess = &dummyLess, *tailEqual = &dummyEqual, *tailGreater = &dummyGreater;

  Node* curr = head;
  while (curr)
  {
    Node* nextNode = curr->next;
    curr->next = nullptr;

    if (curr->value < k)
      tailLess = tailLess->next = curr;
    else if (curr->value == k)
      tailEqual = tailEqual->next = curr;
    else
      tailGreater = tailGreater->next = curr;

    curr = nextNode;
  }

  tailEqual->next = dummyGreater.next;
  tailLess->next = dummyEqual.next ? dummyEqual.next : dummyGreater.next;

  head = dummyLess.next ? dummyLess.next : (dummyEqual.next ? dummyEqual.next : dummyGreater.next);
  tail = (tailGreater != &dummyGreater) ? tailGreater : (tailEqual != &dummyEqual ? tailEqual : tailLess);
}
