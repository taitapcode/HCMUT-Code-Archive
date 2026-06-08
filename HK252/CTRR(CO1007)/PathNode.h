#include <string>
#include <vector>

struct PathNode
{
  std::string name;
  double f, g, h;
  PathNode* next;

  PathNode(std::string name, double f, double g, double h, PathNode* next = nullptr);
};

struct Node
{
  int index;
  double f, g;

  Node(int index, double f, double g) : index(index), f(f), g(g) {};
};

bool comp(const Node& a, const Node& b);
Node pop(std::vector<Node>& os);
