class LLNode
{
public:
  int val;
  LLNode* next;
  LLNode();                       // Constructor: val = 0, next = nullptr
  LLNode(int val, LLNode* next);  // Constructor with customized data
};

LLNode* rotateLinkedList(LLNode* head, int k)
{
  if (!head) return head;

  LLNode *newTail = head, *tail = head;

  int n = 1;
  for (; tail->next; tail = tail->next, n++);

  k = (k % n + n) % n;
  if (!k) return head;

  for (int i = 0; i < n - k - 1; i++, newTail = newTail->next);

  LLNode* newHead = newTail->next;
  newTail->next = nullptr;
  tail->next = head;

  return newHead;
};
