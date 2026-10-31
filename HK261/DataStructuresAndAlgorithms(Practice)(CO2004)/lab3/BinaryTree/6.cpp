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

int sumDigitPath(BTNode* root, int currSum)
{
  if (!root) return 0;

  const int MOD = 27022001;
  currSum = (currSum * 10 + root->val) % MOD;

  if (!root->left && !root->right) return currSum;

  int leftSum = sumDigitPath(root->left, currSum);
  int rightSum = sumDigitPath(root->right, currSum);
  return (leftSum + rightSum) % MOD;
}

int sumDigitPath(BTNode* root)
{
  return sumDigitPath(root, 0);
}
