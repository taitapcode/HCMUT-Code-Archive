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

void freeTree(BSTNode* node)
{
  if (!node) return;
  freeTree(node->left);
  freeTree(node->right);
  delete node;
}

BSTNode* subtreeWithRange(BSTNode* root, int lo, int hi)
{
  if (!root) return nullptr;

  if (root->val < lo)
  {
    BSTNode* right = root->right;
    freeTree(root->left);
    delete root;
    return subtreeWithRange(right, lo, hi);
  }

  if (root->val > hi)
  {
    BSTNode* left = root->left;
    freeTree(root->right);
    delete root;
    return subtreeWithRange(left, lo, hi);
  }

  root->left = subtreeWithRange(root->left, lo, hi);
  root->right = subtreeWithRange(root->right, lo, hi);
  return root;
}
