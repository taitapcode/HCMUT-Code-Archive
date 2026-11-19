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

  bool search(int val)
  {
    if (!root) return false;
    Node *curr = root, *last = nullptr;

    while (curr)
    {
      last = curr;
      if (val == curr->val)
      {
        splay(curr);
        return true;
      }
      else if (val < curr->val)
        curr = curr->pLeft;
      else
        curr = curr->pRight;
    }

    if (last) splay(last);
    return false;
  }
};
