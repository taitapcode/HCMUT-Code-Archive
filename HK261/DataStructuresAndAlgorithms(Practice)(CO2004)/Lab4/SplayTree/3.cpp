class SplayTree
{
  struct Node
  {
    int val;
    Node* pLeft;
    Node* pRight;
    Node* pParent;
    Node(int val = 0, Node* l = nullptr, Node* r = nullptr, Node* par = nullptr) : val(val), pLeft(l), pRight(r), pParent(par) {}
  };
  Node* root;

public:
  void splay(Node* p);

  Node* remove(int val)
  {
    if (!root) return root;

    Node *curr = root, *last;
    while (curr)
    {
      last = curr;
      if (val == curr->val) break;
      if (val < curr->val)
        curr = curr->pLeft;
      else
        curr = curr->pRight;
    }

    splay(last);
    if (root->val != val) return nullptr;

    Node *delNode = root, *leftTree = root->pLeft, *rightTree = root->pRight;
    if (leftTree) leftTree->pParent = nullptr;
    if (rightTree) rightTree->pParent = nullptr;

    if (!leftTree)
      root = rightTree;
    else
    {
      Node* maxLeft = leftTree;
      while (maxLeft->pRight) maxLeft = maxLeft->pRight;

      root = leftTree;
      splay(maxLeft);

      root->pRight = rightTree;
      if (rightTree) rightTree->pParent = root;
    }

    return delNode;
  }
};
