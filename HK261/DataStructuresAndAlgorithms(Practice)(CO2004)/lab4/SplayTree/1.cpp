#include <iostream>
using namespace std;

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

  // print the tree structure for local testing
  void printBinaryTree(string prefix, const Node* root, bool isLeft, bool hasRightSibling)
  {
    if (!root && isLeft && hasRightSibling)
    {
      cout << prefix << "├──\n";
    }
    if (!root) return;
    cout << prefix;
    if (isLeft && hasRightSibling)
      cout << "├──";
    else
      cout << "└──";
    cout << root->val << '\n';
    printBinaryTree(prefix + (isLeft && hasRightSibling ? "|  " : "   "), root->pLeft, true, root->pRight);
    printBinaryTree(prefix + (isLeft && hasRightSibling ? "|  " : "   "), root->pRight, false, root->pRight);
  }

  void printPreorder(Node* p)
  {
    if (!p)
    {
      return;
    }
    cout << p->val << ' ';
    printPreorder(p->pLeft);
    printPreorder(p->pRight);
  }

public:
  SplayTree()
  {
    root = nullptr;
  }
  ~SplayTree()
  {
    // Ignore deleting all nodes in the tree
  }

  void printBinaryTree()
  {
    printBinaryTree("", root, false, false);
  }

  void printPreorder()
  {
    printPreorder(root);
    cout << "\n";
  }

  // ===================================
  void splay(Node* p)
  {
    // To Do
  }

  void insert(int val)
  {
    // To Do
  }
  // ===================================
};
