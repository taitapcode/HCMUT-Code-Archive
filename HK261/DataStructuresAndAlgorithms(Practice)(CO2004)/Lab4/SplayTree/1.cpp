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
  void rotateLeft(Node* x)
  {
    if (!x || !x->pRight) return;
    Node* y = x->pRight;

    x->pRight = y->pLeft;
    if (y->pLeft) y->pLeft->pParent = x;

    y->pParent = x->pParent;
    if (!x->pParent)
      root = y;
    else if (x == x->pParent->pLeft)
      x->pParent->pLeft = y;
    else
      x->pParent->pRight = y;

    y->pLeft = x;
    x->pParent = y;
  }

  void rotateRight(Node* x)
  {
    if (!x || !x->pLeft) return;
    Node* y = x->pLeft;

    x->pLeft = y->pRight;
    if (y->pRight) y->pRight->pParent = x;

    y->pParent = x->pParent;
    if (!x->pParent)
      root = y;
    else if (x == x->pParent->pLeft)
      x->pParent->pLeft = y;
    else
      x->pParent->pRight = y;

    y->pRight = x;
    x->pParent = y;
  }

  void splay(Node* p)
  {
    if (!p) return;
    while (p->pParent)
    {
      Node *par = p->pParent, *gpar = par->pParent;

      if (!gpar)
      {
        if (p == par->pLeft)
          rotateRight(par);
        else
          rotateLeft(par);
      }
      else if (par == gpar->pLeft && p == par->pLeft)
      {
        rotateRight(gpar);
        rotateRight(par);
      }
      else if (par == gpar->pLeft && p == par->pRight)
      {
        rotateLeft(par);
        rotateRight(gpar);
      }
      else if (par == gpar->pRight && p == par->pRight)
      {
        rotateLeft(gpar);
        rotateLeft(par);
      }
      else if (par == gpar->pRight && p == par->pLeft)
      {
        rotateRight(par);
        rotateLeft(gpar);
      }
    }

    root = p;
  }

  void insert(int val)
  {
    Node* newNode = new Node(val);
    if (!root)
    {
      root = newNode;
      return;
    }

    Node *curr = root, *par = nullptr;
    while (curr)
    {
      par = curr;
      if (val < curr->val)
        curr = curr->pLeft;
      else
        curr = curr->pRight;
    }

    newNode->pParent = par;
    if (val < par->val)
      par->pLeft = newNode;
    else
      par->pRight = newNode;

    splay(newNode);
  }
};
