template <class T>
class BinarySearchTree
{
public:
  class Node
  {
  private:
    T value;
    Node *pLeft, *pRight;
    friend class BinarySearchTree<T>;
  };

private:
  Node* root;

public:
  // Helping function
  Node* add(Node* root, T value)
  {
    if (!root) return new Node(value);
    if (value <= root->value) root->pLeft = add(root->pLeft, value);
    if (value > root->value) root->pRight = add(root->pRight, value);
    return root;
  }

  Node* deleteNode(Node* root, T value)
  {
    if (!root) return nullptr;
    if (value < root->value)
      root->pLeft = deleteNode(root->pLeft, value);
    else if (value > root->value)
      root->pRight = deleteNode(root->pRight, value);
    else
    {
      if (!root->pLeft)
      {
        Node* newRoot = root->pRight;
        delete root;
        return newRoot;
      }
      if (!root->pRight)
      {
        Node* newRoot = root->pLeft;
        delete root;
        return newRoot;
      }

      Node* succ = root->pRight;
      while (succ && succ->pLeft) succ = succ->pLeft;
      root->value = succ->value;
      root->pRight = deleteNode(root->pRight, succ->value);
    }

    return root;
  }

  void add(T value)
  {
    // TODO
    root = add(root, value);
  }

  void deleteNode(T value)
  {
    // TODO
    root = deleteNode(root, value);
  }
};
