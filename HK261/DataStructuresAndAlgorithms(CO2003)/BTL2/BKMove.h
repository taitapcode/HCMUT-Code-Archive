#ifndef BKMOVE_A2_H
#define BKMOVE_A2_H

#include <string>
#include <vector>

struct StopInfo
{
  std::string stopId;
  std::string displayName;
  std::string searchKey;
};

struct ArrivalEvent
{
  std::string eventId;
  std::string stopId;
  std::string routeId;
  int arrivalTime;
};

class BKMove
{
public:
  BKMove();
  ~BKMove();

  BKMove(const BKMove&) = delete;
  BKMove& operator=(const BKMove&) = delete;

  // Q3: Bus Stop Finder
  bool addStop(const StopInfo& stop);
  bool removeStop(const std::string& stopId);
  bool findStop(const std::string& stopId, StopInfo& out) const;
  int getStopCount() const;
  std::vector<StopInfo> findStops(
      const std::string& prefix, int page, int pageSize) const;

  // Q4: Live Arrival Board
  bool scheduleArrival(const ArrivalEvent& event);
  bool peekNextArrival(const std::string& stopId,
                       ArrivalEvent& out) const;
  bool processNextArrival(const std::string& stopId,
                          ArrivalEvent& out);
  bool rescheduleArrival(const std::string& eventId,
                         int newArrivalTime);
  bool cancelArrival(const std::string& eventId);

private:
  // Q3/Q4: Students propose their own private data members and helpers.
  // BKMove must manage registered stops and pending arrival events.
  // You may add classes/structs, extend BST/AVL/Heap, or use inheritance.
  // Keep the public API and the provided data-record fields unchanged.
  // All implementations must stay within the five permitted source files.
  struct StopNode
  {
    StopInfo info;
    int pending;
    explicit StopNode(const StopInfo& s) : info(s), pending(0) {}
  };

  class StopIndex
  {
  private:
    struct Node
    {
      std::string key;
      StopNode* stop;
      Node* left;
      Node* right;
      int height;
      int count;
      Node(const std::string& k, StopNode* s)
          : key(k), stop(s), left(nullptr), right(nullptr), height(1), count(1) {}
    };

    Node* root;

    static int height(const Node* node)
    {
      return node ? node->height : 0;
    }

    static int size(const Node* n)
    {
      return n ? n->count : 0;
    }

    static void refresh(Node* node)
    {
      if (!node) return;
      int hl = height(node->left);
      int hr = height(node->right);
      node->height = (hl > hr ? hl : hr) + 1;
      node->count = size(node->left) + size(node->right) + 1;
    }

    static Node* rotateLeft(Node* node)
    {
      Node* child = node->right;
      node->right = child->left;
      child->left = node;
      refresh(node);
      refresh(child);
      return child;
    }

    static Node* rotateRight(Node* node)
    {
      Node* child = node->left;
      node->left = child->right;
      child->right = node;
      refresh(node);
      refresh(child);
      return child;
    }

    static Node* rebalance(Node* node)
    {
      if (!node) return nullptr;
      refresh(node);
      int bf = height(node->right) - height(node->left);
      if (bf >= 2)
      {
        if (height(node->right->right) < height(node->right->left))
          node->right = rotateRight(node->right);
        return rotateLeft(node);
      }
      if (bf <= -2)
      {
        if (height(node->left->left) < height(node->left->right))
          node->left = rotateLeft(node->left);
        return rotateRight(node);
      }
      return node;
    }

    static Node* insert(Node* node, const std::string& key, StopNode* stop, bool& success)
    {
      if (!node)
      {
        success = true;
        return new Node(key, stop);
      }
      if (key < node->key)
        node->left = insert(node->left, key, stop, success);
      else if (node->key < key)
        node->right = insert(node->right, key, stop, success);
      else
        return node;
      return rebalance(node);
    }

    static Node* detachMin(Node* node)
    {
      if (!node->left)
      {
        Node* right = node->right;
        delete node;
        return right;
      }
      node->left = detachMin(node->left);
      return rebalance(node);
    }

    static Node* erase(Node* node, const std::string& key, bool& removed)
    {
      if (!node) return nullptr;
      if (key < node->key)
        node->left = erase(node->left, key, removed);
      else if (node->key < key)
        node->right = erase(node->right, key, removed);
      else
      {
        removed = true;
        if (!node->left || !node->right)
        {
          Node* child = node->left ? node->left : node->right;
          delete node;
          return child;
        }
        Node* succ = node->right;
        while (succ->left) succ = succ->left;
        node->key = succ->key;
        node->stop = succ->stop;
        node->right = detachMin(node->right);
      }
      return rebalance(node);
    }

    static void clear(Node* node, void (*dispose)(StopNode*))
    {
      if (!node) return;
      clear(node->left, dispose);
      clear(node->right, dispose);
      if (dispose) dispose(node->stop);
      delete node;
    }

  public:
    StopIndex() : root(nullptr) {}

    StopIndex(const StopIndex&) = delete;
    StopIndex& operator=(const StopIndex&) = delete;
    ~StopIndex() { clear(nullptr); }

    bool empty() const
    {
      return !root;
    }

    int size() const
    {
      return root ? root->count : 0;
    }

    void clear(void (*dispose)(StopNode*))
    {
      clear(root, dispose);
      root = nullptr;
    }

    bool erase(const std::string& key)
    {
      bool sucess = false;
      root = erase(root, key, sucess);
      return sucess;
    }

    bool insert(const std::string& key, StopNode* stop)
    {
      bool success = false;
      root = insert(root, key, stop, success);
      return success;
    }

    StopNode* find(const std::string& key) const
    {
      Node* node = root;
      while (node)
      {
        if (key < node->key)
          node = node->left;
        else if (node->key < key)
          node = node->right;
        else
          return node->stop;
      }
      return nullptr;
    }

    int rank(const std::string& target) const
    {
      int rank = 0;
      Node* node = root;
      while (node)
      {
        if (node->key < target)
        {
          rank += size(node->left) + 1;
          node = node->right;
        }
        else
        {
          node = node->left;
        }
      }
      return rank;
    }

    void collect(int start, int limit, std::vector<StopNode*>& out) const
    {
      std::vector<Node*> st;
      Node* curr = root;
      int rest = start;
      while (curr)
      {
        int leftSize = size(curr->left);
        if (rest < leftSize)
        {
          st.push_back(curr);
          curr = curr->left;
        }
        else if (rest == leftSize)
          break;
        else
        {
          rest -= leftSize + 1;
          curr = curr->right;
        }
      }

      while (curr && (int)out.size() < limit)
      {
        out.push_back(curr->stop);
        if (curr->right)
        {
          curr = curr->right;
          while (curr->left)
          {
            st.push_back(curr);
            curr = curr->left;
          }
        }
        else if (!st.empty())
        {
          curr = st.back();
          st.pop_back();
        }
        else
          curr = nullptr;
      }
    }
  };

  static void disposeStop(StopNode* stop) { delete stop; }
  static bool nextPrefix(const std::string& prefix, std::string& upper);

  StopIndex byKey;
  StopIndex byId;
};

#endif  // BKMOVE_A2_H
