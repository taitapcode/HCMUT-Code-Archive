#include <iostream>
#include <queue>
using namespace std;
#define SEPARATOR "#<ab@17943918#@>#"

enum BalanceValue
{
  LH = -1,
  EH = 0,
  RH = 1
};

template <class T>
class AVLTree
{
public:
  class Node;

private:
  Node* root;

public:
  AVLTree() : root(nullptr) {}
  ~AVLTree() {}

  void printInorder(Node* node)
  {
    if (!node) return;
    printInorder(node->pLeft);
    cout << node->data << " ";
    printInorder(node->pRight);
  }

  bool search(Node* node, const T& value)
  {
    if (!node) return false;
    if (value == node->data) return true;
    if (value < node->data) return search(node->pLeft, value);
    return search(node->pRight, value);
  }

  void printInorder()
  {
    // TODO
    printInorder(this->root);
  }

  bool search(const T& value)
  {
    // TODO
    return search(this->root, value);
  }

  class Node
  {
  private:
    T data;
    Node *pLeft, *pRight;
    BalanceValue balance;
    friend class AVLTree<T>;

  public:
    Node(T value) : data(value), pLeft(NULL), pRight(NULL), balance(EH) {}
    ~Node() {}
  };
};
