template <class T>
class BinarySearchTree
{
public:
  class Node
  {
    T value;
    Node *pLeft, *pRight;
    friend class BinarySearchTree<T>;
  };

private:
  Node* root;

public:
  // STUDENT ANSWER BEGIN
  T getMin()
  {
    // TODO: return the minimum values of nodes in the tree.
    Node* minNode = root;
    while (minNode && minNode->pLeft) minNode = minNode->pLeft;
    return minNode ? minNode->value : T();
  }

  T getMax()
  {
    // TODO: return the maximum values of nodes in the tree.
    Node* maxNode = root;
    while (maxNode && maxNode->pRight) maxNode = maxNode->pRight;
    return maxNode ? maxNode->value : T();
  }
  // STUDENT ANSWER END
};
