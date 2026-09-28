class LLNode
{
public:
  int val;
  LLNode* next;
  LLNode();
  LLNode(int val, LLNode* next);
};

LLNode* reverseLinkedList(LLNode* head)
{
  LLNode *prev = nullptr, *curr = head;

  while (curr != nullptr)
  {
    LLNode* next = curr->next;
    curr->next = prev;
    prev = curr;
    curr = next;
  }

  return prev;
}
