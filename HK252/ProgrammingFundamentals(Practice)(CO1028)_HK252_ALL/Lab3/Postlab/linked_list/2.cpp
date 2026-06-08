#include "linked_list.h"

void mergeList(node* head1, node* head2)
{
  while (head1->next != nullptr) head1 = head1->next;
  head1->next = head2;
}
