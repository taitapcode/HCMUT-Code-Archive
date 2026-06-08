#include "linked_list.h"

int countNode(node* head)
{
  int count = 0;
  while(head) head = head->next, count++;
  return count;
}
