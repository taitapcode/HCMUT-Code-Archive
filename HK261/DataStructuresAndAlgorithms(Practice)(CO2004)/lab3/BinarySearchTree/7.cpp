class BSTNode
{
public:
  int val;
  BSTNode* left;
  BSTNode* right;
  BSTNode()
  {
    this->left = this->right = nullptr;
  }
  BSTNode(int val)
  {
    this->val = val;
    this->left = this->right = nullptr;
  }
  BSTNode(int val, BSTNode*& left, BSTNode*& right)
  {
    this->val = val;
    this->left = left;
    this->right = right;
  }
};

bool find(BSTNode* node, int& k, int& result)
{
  if (!node) return false;
  if (find(node->left, k, result)) return true;
  if (--k == 0)
  {
    result = node->val;
    return true;
  }

  return find(node->right, k, result);
}

int kthSmallest(BSTNode* root, int k)
{
  // STUDENT ANSWER
  int result = -1;
  find(root, k, result);
  return result;
}
