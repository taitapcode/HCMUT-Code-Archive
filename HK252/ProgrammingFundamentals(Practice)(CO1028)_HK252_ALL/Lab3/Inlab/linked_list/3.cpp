#include "linked_list.h"

void replace(node* head, int position, int value)
{
  if (position < 0) return;
  while(position-- && head) head = head->next;
  if (head) head->data = value;
}
