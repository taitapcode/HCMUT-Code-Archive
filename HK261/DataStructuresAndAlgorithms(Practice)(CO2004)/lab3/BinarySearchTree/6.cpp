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

int singleChild(BSTNode* root)
{
  // STUDENT ANSWER
  if (!root) return 0;
  return ((!!root->left) != (!!root->right)) + singleChild(root->left) + singleChild(root->right);
}
