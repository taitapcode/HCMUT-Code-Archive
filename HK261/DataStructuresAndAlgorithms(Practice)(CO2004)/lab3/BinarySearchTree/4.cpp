#include <algorithm>
#include <queue>
#include <vector>
using namespace std;

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

vector<int> levelAlterTraverse(BTNode* root)
{
  vector<int> res;
  if (!root) return res;

  bool isReverse = false;
  queue<BTNode*> q;
  q.push(root);

  while (!q.empty())
  {
    int qSize = q.size();
    vector<int> level;
    level.reserve(qSize);
    for (int i = 0; i < qSize; i++)
    {
      BTNode* node = q.front();
      q.pop();
      level.push_back(node->val);

      if (node->left) q.push(node->left);
      if (node->right) q.push(node->right);
    }
    if (isReverse) reverse(level.begin(), level.end());
    res.insert(res.end(), level.begin(), level.end());
    isReverse = !isReverse;
  }

  return res;
}
