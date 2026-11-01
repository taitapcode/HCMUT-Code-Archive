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
  struct StopNode;

  struct EventSlot
  {
    ArrivalEvent event;
    int pos;
    StopNode* owner;
    EventSlot(const ArrivalEvent& e, StopNode* o)
        : event(e), pos(-1), owner(o) {}
  };

  struct StopNode
  {
    StopInfo info;
    int pending;
    std::vector<EventSlot*> board;
    explicit StopNode(const StopInfo& s) : info(s), pending(0) {}
  };

  template <class T>
  class Index
  {
  private:
    struct Node
    {
      std::string key;
      T* item;
      Node* left;
      Node* right;
      int height;
      int count;
      Node(const std::string& k, T* value)
          : key(k), item(value), left(nullptr), right(nullptr), height(1), count(1) {}
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

    static Node* insert(Node* node, const std::string& key, T* item, bool& success)
    {
      if (!node)
      {
        success = true;
        return new Node(key, item);
      }
      if (key < node->key)
        node->left = insert(node->left, key, item, success);
      else if (node->key < key)
        node->right = insert(node->right, key, item, success);
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
        node->item = succ->item;
        node->right = detachMin(node->right);
      }
      return rebalance(node);
    }

    static void clear(Node* node, void (*dispose)(T*))
    {
      if (!node) return;
      clear(node->left, dispose);
      clear(node->right, dispose);
      if (dispose) dispose(node->item);
      delete node;
    }

  public:
    Index() : root(nullptr) {}

    Index(const Index&) = delete;
    Index& operator=(const Index&) = delete;
    ~Index() { clear(nullptr); }

    bool empty() const
    {
      return !root;
    }

    int size() const
    {
      return root ? root->count : 0;
    }

    void clear(void (*dispose)(T*))
    {
      clear(root, dispose);
      root = nullptr;
    }

    bool erase(const std::string& key)
    {
      bool success = false;
      root = erase(root, key, success);
      return success;
    }

    bool insert(const std::string& key, T* item)
    {
      bool success = false;
      root = insert(root, key, item, success);
      return success;
    }

    T* find(const std::string& key) const
    {
      Node* node = root;
      while (node)
      {
        if (key < node->key)
          node = node->left;
        else if (node->key < key)
          node = node->right;
        else
          return node->item;
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

    void collect(int start, int limit, std::vector<T*>& out) const
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
        out.push_back(curr->item);
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

  static bool earlier(const EventSlot* a, const EventSlot* b)
  {
    if (a->event.arrivalTime != b->event.arrivalTime)
      return a->event.arrivalTime < b->event.arrivalTime;
    return a->event.eventId < b->event.eventId;
  }

  static void swapSlots(StopNode* stop, int i, int j)
  {
    EventSlot* tmp = stop->board[i];
    stop->board[i] = stop->board[j];
    stop->board[j] = tmp;
    stop->board[i]->pos = i;
    stop->board[j]->pos = j;
  }

  static void reheapUp(StopNode* stop, int idx)
  {
    while (idx > 0)
    {
      int parent = (idx - 1) / 2;
      if (!earlier(stop->board[idx], stop->board[parent])) break;
      swapSlots(stop, idx, parent);
      idx = parent;
    }
  }

  static void reheapDown(StopNode* stop, int idx)
  {
    int n = (int)stop->board.size();
    while (true)
    {
      int left = 2 * idx + 1;
      int right = left + 1;
      int best = idx;
      if (left < n && earlier(stop->board[left], stop->board[best])) best = left;
      if (right < n && earlier(stop->board[right], stop->board[best])) best = right;
      if (best == idx) break;
      swapSlots(stop, idx, best);
      idx = best;
    }
  }

  static void detachSlot(StopNode* stop, int idx)
  {
    int last = (int)stop->board.size() - 1;
    if (idx != last)
    {
      EventSlot* moved = stop->board[last];
      stop->board[idx] = moved;
      moved->pos = idx;
      stop->board.pop_back();
      reheapUp(stop, idx);
      reheapDown(stop, moved->pos);
    }
    else
      stop->board.pop_back();
    --stop->pending;
  }

  static void disposeStop(StopNode* stop) { delete stop; }
  static void disposeEvent(EventSlot* slot) { delete slot; }
  static bool nextPrefix(const std::string& prefix, std::string& upper);

  Index<StopNode> byKey;
  Index<StopNode> byId;
  Index<EventSlot> byEvent;
};

#endif  // BKMOVE_A2_H
