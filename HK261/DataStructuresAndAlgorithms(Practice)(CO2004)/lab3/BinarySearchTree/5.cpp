#include <stack>
using namespace std;

class BTNode
{
public:
  int val;
  BTNode* left;
  BTNode* right;
  BTNode()
  {
    this->left = this->right = NULL;
  }
  BTNode(int val)
  {
    this->val = val;
    this->left = this->right = NULL;
  }
  BTNode(int val, BTNode*& left, BTNode*& right)
  {
    this->val = val;
    this->left = left;
    this->right = right;
  }
};

int rangeCount(BTNode* root, int lo, int hi)
{
  if (!root) return 0;

  int count = 0;
  stack<BTNode*> st;
  st.push(root);

  while (!st.empty())
  {
    BTNode* cur = st.top();
    st.pop();

    if (cur->val >= lo && cur->val <= hi) count++;
    if (cur->left && cur->val > lo) st.push(cur->left);
    if (cur->right && cur->val <= hi) st.push(cur->right);
  }

  return count;
}
