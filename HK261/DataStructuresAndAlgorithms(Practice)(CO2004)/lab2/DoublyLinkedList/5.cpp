struct ListNode
{
  int val;
  ListNode* left;
  ListNode* right;
  ListNode(int x = 0, ListNode* l = nullptr, ListNode* r = nullptr) : val(x), left(l), right(r) {}
};

ListNode* reverse(ListNode* head, int a, int b)
{
  if (!head || a == b) return head;
  ListNode *NodeA = head, *NodeB = head;
  for (int i = 1; i < b; i++)
  {
    if (i < a) NodeA = NodeA->right;
    NodeB = NodeB->right;
  }

  ListNode *beforeA = NodeA->left, *afterB = NodeB->right;

  ListNode* curr = NodeA;
  while (curr != afterB)
  {
    ListNode* next = curr->right;
    curr->right = curr->left;
    curr->left = next;
    curr = next;
  }

  NodeB->left = beforeA;
  if (beforeA) beforeA->right = NodeB;

  NodeA->right = afterB;
  if (afterB) afterB->left = NodeA;

  return (a == 1) ? NodeB : head;
}
