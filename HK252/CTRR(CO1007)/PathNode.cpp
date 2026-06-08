#include "PathNode.h"

#include <algorithm>

PathNode::PathNode(std::string name, double f, double g, double h, PathNode* next)
    : name(name), f(f), g(g), h(h), next(next) {};

bool comp(const Node& a, const Node& b)
{
  if (std::abs(a.f - b.f) < 1e-9)
  {
    if (std::abs(a.g - b.g) < 1e-9) return a.index > b.index;
    return a.g < b.g;
  }
  return a.f > b.f;
}

Node pop(std::vector<Node>& os)
{
  auto max_it = std::max_element(os.begin(), os.end(), comp);
  std::iter_swap(max_it, os.end() - 1);
  Node max_node = std::move(os.back());
  os.pop_back();
  return max_node;
}
