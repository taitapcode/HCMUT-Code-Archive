
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

void printNSpace(int n)
{
  for (int i = 0; i < n - 1; i++)
    cout << " ";
}

void printInteger(int& n)
{
  cout << n << " ";
}

template <class T>
class AVLTree
{
public:
  class Node;

private:
  Node* root;

protected:
  int getHeightRec(Node* node)
  {
    if (node == NULL)
      return 0;
    int lh = this->getHeightRec(node->pLeft);
    int rh = this->getHeightRec(node->pRight);
    return (lh > rh ? lh : rh) + 1;
  }

public:
  AVLTree() : root(nullptr) {}
  ~AVLTree() {}
  int getHeight()
  {
    return this->getHeightRec(this->root);
  }
  void printTreeStructure()
  {
    int height = this->getHeight();
    if (this->root == NULL)
    {
      cout << "NULL\n";
      return;
    }
    queue<Node*> q;
    q.push(root);
    Node* temp;
    int count = 0;
    int maxNode = 1;
    int level = 0;
    int space = pow(2, height);
    printNSpace(space / 2);
    while (!q.empty())
    {
      temp = q.front();
      q.pop();
      if (temp == NULL)
      {
        cout << " ";
        q.push(NULL);
        q.push(NULL);
      }
      else
      {
        cout << temp->data;
        q.push(temp->pLeft);
        q.push(temp->pRight);
      }
      printNSpace(space);
      count++;
      if (count == maxNode)
      {
        cout << endl;
        count = 0;
        maxNode *= 2;
        level++;
        space /= 2;
        printNSpace(space / 2);
      }
      if (level == height)
        return;
    }
  }

  // Helping functions
  int getBalance(Node* root)
  {
    return getHeightRec(root->pLeft) - getHeightRec(root->pRight);
  }

  void updateBalance(Node* root)
  {
    int bf = getBalance(root);
    root->balance = bf > 0 ? LH : (bf < 0 ? RH : EH);
  }

  Node* leftRotate(Node* root)
  {
    Node* newRoot = root->pRight;
    root->pRight = newRoot->pLeft;
    newRoot->pLeft = root;
    updateBalance(root);
    updateBalance(newRoot);
    return newRoot;
  }

  Node* rightRotate(Node* root)
  {
    Node* newRoot = root->pLeft;
    root->pLeft = newRoot->pRight;
    newRoot->pRight = root;
    updateBalance(root);
    updateBalance(newRoot);
    return newRoot;
  }

  Node* findMax(Node* root)
  {
    while (root->pRight) root = root->pRight;
    return root;
  }

  Node* remove(Node* root, const T& value)
  {
    if (!root) return nullptr;

    if (value < root->data)
      root->pLeft = remove(root->pLeft, value);
    else if (value > root->data)
      root->pRight = remove(root->pRight, value);
    else
    {
      if (!root->pLeft || !root->pRight)
      {
        Node* newRoot = root->pLeft ? root->pLeft : root->pRight;
        delete root;
        return newRoot;
      }

      Node* pred = findMax(root->pLeft);
      root->data = pred->data;
      root->pLeft = remove(root->pLeft, pred->data);
    }

    updateBalance(root);
    int bf = getBalance(root);

    if (bf > 1)
    {
      if (getBalance(root->pLeft) < 0) root->pLeft = leftRotate(root->pLeft);
      return rightRotate(root);
    }
    if (bf < -1)
    {
      if (getBalance(root->pRight) > 0) root->pRight = rightRotate(root->pRight);
      return leftRotate(root);
    }

    return root;
  }

  void remove(const T& value)
  {
    // TODO
    root = remove(root, value);
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
