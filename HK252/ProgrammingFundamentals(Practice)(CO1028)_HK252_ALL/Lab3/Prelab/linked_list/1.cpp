#include "linked_list.h"

bool isEqual(node *head1, node *head2)
{
  while(head1 && head2)
  {
    if (head1->data != head2->data) return 0;
    head1 = head1->next;
    head2 = head2->next;
  }

  return !head1 && !head2;
}

