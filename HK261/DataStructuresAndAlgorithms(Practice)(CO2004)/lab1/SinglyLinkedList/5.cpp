class LLNode
{
public:
  int val;
  LLNode* next;
  LLNode();
  LLNode(int val, LLNode* next);
};

LLNode* addLinkedList(LLNode* l0, LLNode* l1)
{
  LLNode *result = nullptr, *curr = nullptr, *newNode = nullptr;
  int reminder = 0;

  while (l0 || l1)
  {
    int sum = reminder;
    if (l0)
    {
      sum += l0->val;
      l0 = l0->next;
    }
    if (l1)
    {
      sum += l1->val;
      l1 = l1->next;
    }
    reminder = sum / 10;
    sum %= 10;

    newNode = new LLNode(sum, nullptr);
    if (!result)
      result = curr = newNode;
    else
      curr = curr->next = newNode;
  }

  if (reminder)
  {
    newNode = new LLNode(reminder, nullptr);
    curr->next = newNode;
  }

  return result;
}
