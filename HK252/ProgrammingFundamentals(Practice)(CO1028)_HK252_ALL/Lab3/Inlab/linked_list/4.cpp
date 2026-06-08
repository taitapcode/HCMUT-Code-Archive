#include "linked_list.h"

int searchLinkedList(node* head, int key)
{
  int idx = 0;
  while(head)
  {
    if (head->data == key) return idx;
    head = head->next;
    ++idx;
  }

  return -1;
}
