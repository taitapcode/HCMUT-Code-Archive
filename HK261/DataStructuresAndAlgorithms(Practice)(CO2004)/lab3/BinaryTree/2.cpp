#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

template <class K, class V>
class BinaryTree
{
public:
  class Node;

private:
  Node* root;

public:
  BinaryTree() : root(nullptr) {}
  ~BinaryTree()
  {
    // You have to delete all Nodes in BinaryTree. However in this task, you can ignore it.
  }
  class Node
  {
  private:
    K key;
    V value;
    Node *pLeft, *pRight;
    friend class BinaryTree<K, V>;

  public:
    Node(K key, V value) : key(key), value(value), pLeft(NULL), pRight(NULL) {}
    ~Node() {}
  };
  void addNode(string posFromRoot, K key, V value)
  {
    if (posFromRoot == "")
    {
      this->root = new Node(key, value);
      return;
    }
    Node* walker = this->root;
    int l = posFromRoot.length();
    for (int i = 0; i < l - 1; i++)
    {
      if (!walker)
        return;
      if (posFromRoot[i] == 'L')
        walker = walker->pLeft;
      if (posFromRoot[i] == 'R')
        walker = walker->pRight;
    }
    if (posFromRoot[l - 1] == 'L')
      walker->pLeft = new Node(key, value);
    if (posFromRoot[l - 1] == 'R')
      walker->pRight = new Node(key, value);
  }
  // STUDENT ANSWER BEGIN
  int getHeight(Node* root)
  {
    if (!root) return 0;
    return 1 + max(getHeight(root->pLeft), getHeight(root->pRight));
  }
  int getHeight()
  {
    // TODO: return height of the binary tree.
    return getHeight(root);
  }

  void preOrder(Node* node, stringstream& ss)
  {
    if (!node) return;
    ss << node->value << " ";
    preOrder(node->pLeft, ss);
    preOrder(node->pRight, ss);
  }

  string preOrder()
  {
    // TODO: return the sequence of values of nodes in pre-order.
    stringstream ss;
    preOrder(this->root, ss);
    string res = ss.str();
    if (!res.empty()) res.pop_back();
    return res;
  }

  void inOrder(Node* node, stringstream& ss)
  {
    if (!node) return;
    inOrder(node->pLeft, ss);
    ss << node->value << " ";
    inOrder(node->pRight, ss);
  }

  string inOrder()
  {
    // TODO: return the sequence of values of nodes in in-order.
    stringstream ss;
    inOrder(this->root, ss);
    string res = ss.str();
    if (!res.empty()) res.pop_back();
    return res;
  }

  void postOrder(Node* node, stringstream& ss)
  {
    if (!node) return;
    postOrder(node->pLeft, ss);
    postOrder(node->pRight, ss);
    ss << node->value << " ";
  }

  string postOrder()
  {
    // TODO: return the sequence of values of nodes in post-order.
    stringstream ss;
    postOrder(this->root, ss);
    string res = ss.str();
    if (!res.empty()) res.pop_back();
    return res;
  }
  // STUDENT ANSWER END
};
