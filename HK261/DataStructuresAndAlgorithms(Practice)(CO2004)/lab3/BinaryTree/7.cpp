class BTNode
{
public:
  int val;
  BTNode* left;
  BTNode* right;
  BTNode()
  {
    this->left = this->right = nullptr;
  }
  BTNode(int val)
  {
    this->val = val;
    this->left = this->right = nullptr;
  }
  BTNode(int val, BTNode*& left, BTNode*& right)
  {
    this->val = val;
    this->left = left;
    this->right = right;
  }
};

BTNode* helper(BTNode* root, int a, int b)
{
  if (!root || root->val == a || root->val == b) return root;

  BTNode* left = helper(root->left, a, b);
  BTNode* right = helper(root->right, a, b);

  if (left && right) return root;

  return left ? left : right;
}

int lowestAncestor(BTNode* root, int a, int b)
{
  BTNode* lca = helper(root, a, b);
  return lca ? lca->val : -1;
}
