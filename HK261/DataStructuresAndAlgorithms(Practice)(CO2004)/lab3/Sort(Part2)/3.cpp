struct ListNode
{
  int val;
  ListNode* next;
  ListNode(int _val = 0, ListNode* _next = nullptr) : val(_val), next(_next) {}
};

ListNode* mergeLists(ListNode* a, ListNode* b)
{
  ListNode dummy;
  ListNode* tail = &dummy;

  while (a && b)
  {
    if (a->val <= b->val)
    {
      tail->next = a;
      a = a->next;
    }
    else
    {
      tail->next = b;
      b = b->next;
    }

    tail = tail->next;
  }

  tail->next = a ? a : b;
  return dummy.next;
}

ListNode* mergeSortList(ListNode* head)
{
  if (!head || !head->next) return head;

  ListNode* slow = head;
  ListNode* fast = head->next;

  while (fast && fast->next)
  {
    slow = slow->next;
    fast = fast->next->next;
  }

  ListNode* second = slow->next;
  slow->next = nullptr;

  ListNode* left = mergeSortList(head);
  ListNode* right = mergeSortList(second);

  return mergeLists(left, right);
}
