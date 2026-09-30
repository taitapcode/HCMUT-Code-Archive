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
  Node *lower = nullptr, *equal = nullptr, *higher = nullptr;
  Node *curr = head, *currLower = nullptr, *currEqual = nullptr, *currHigher = nullptr;
  while (curr != nullptr)
  {
    if (curr->value < k)
    {
      if (!lower)
        currLower = lower = curr;
      else
        currLower = currLower->next = curr;
    }
    else if (curr->value == k)
    {
      if (!equal)
        currEqual = equal = curr;
      else
        currEqual = currEqual->next = curr;
    }
    else
    {
      if (higher == nullptr)
        currHigher = higher = curr;
      else
        currHigher = currHigher->next = curr;
    }

    curr = curr->next;
  }

  if (equal) currLower->next = equal;
  equal->next = higher;
  currHigher->next = nullptr;

  head = lower;
  tail = currHigher;
}
