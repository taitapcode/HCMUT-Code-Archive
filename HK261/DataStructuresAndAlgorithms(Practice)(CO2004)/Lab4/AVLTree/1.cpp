#include <math.h>

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
  Node* rotateLeft(Node* root)
  {
    // TODO: Rotate and return new root after rotate
    Node* newRoot = root->pRight;
    root->pRight = newRoot->pLeft;
    newRoot->pLeft = root;
    return newRoot;
  };

  Node* rotateRight(Node* root)
  {
    // TODO: Rotate and return new root after rotate
    Node* newRoot = root->pLeft;
    root->pLeft = newRoot->pRight;
    newRoot->pRight = root;
    return newRoot;
  };

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
