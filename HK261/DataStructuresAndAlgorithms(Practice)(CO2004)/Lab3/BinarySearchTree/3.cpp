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
  // STUDENT ANSWER BEGIN
  bool find(T i)
  {
    // TODO: return true if value i is in the tree; otherwise, return false.
    Node* curr = root;
    while (curr)
    {
      if (curr->value == i) return true;
      curr = (i < curr->value) ? curr->pLeft : curr->pRight;
    }

    return false;
  }

  T sum(Node* root, T l, T r)
  {
    if (!root) return T();
    T total = T();
    if (l <= root->value && root->value <= r) total += root->value;
    if (l <= root->value) total += sum(root->pLeft, l, r);
    if (root->value <= r) total += sum(root->pRight, l, r);
    return total;
  }

  T sum(T l, T r)
  {
    // TODO: return the sum of all element in the tree has value in range [l,r].
    return sum(root, l, r);
  }
  // STUDENT ANSWER END
};
