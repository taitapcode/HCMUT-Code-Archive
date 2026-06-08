#include "linked_list.h"

node *insertNode(node *head, node *newNode, int position)
{
  if (position < 1) return head;

  node *curr = head;
  while(--position > 1 && curr->next != nullptr) curr = curr->next;
  newNode->next = curr->next;
  curr->next = newNode;

  return head;
}
