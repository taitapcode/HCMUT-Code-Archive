#include <algorithm>
#include <queue>

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

struct Node
{
  BTNode* node;
  int depth, sum;
};

int longestPathSum(BTNode* root)
{
  if (!root) return 0;

  queue<Node> que;
  que.push({root, 1, root->val});

  int maxDepth = 0, maxSum = 0;

  while (!que.empty())
  {
    Node curr = que.front();
    int depth = curr.depth, sum = curr.sum;
    BTNode* node = curr.node;
    que.pop();

    if (!node->left && !node->right && (depth > maxDepth || (depth == maxDepth && sum > maxSum)))
    {
      maxDepth = depth;
      maxSum = sum;
    }

    if (node->left) que.push({node->left, depth + 1, sum + node->left->val});
    if (node->right) que.push({node->right, depth + 1, sum + node->right->val});
  }

  return maxSum;
}
