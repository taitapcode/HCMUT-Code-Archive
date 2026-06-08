#include "linked_list.h"

node *createLinkedList(int n)
{
  int x;
  node *head = nullptr;

  while(n--)
  {
    cin >> x;
    node *newhead = new node{x, head};
    head = newhead;
  }

  return head;
}
