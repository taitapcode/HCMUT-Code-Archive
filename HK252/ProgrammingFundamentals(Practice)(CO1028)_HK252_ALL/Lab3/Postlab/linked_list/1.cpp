#include "linked_list.h"

node* evenThenOddLinkedList(node *head)
{
  node *eHead = nullptr, *oHead = nullptr, *eTail = nullptr, *oTail = nullptr;

  while(head != nullptr)
  {
    int val = head->data;
    if (val & 1)
    {
      if (oHead == nullptr) oHead = oTail = head;
      else oTail = oTail->next = head;
    }
    else
    {
      if (eHead == nullptr) eHead = eTail = head;
      else eTail = eTail->next = head;
    }

    head = head->next;
  }

  if (oTail != nullptr) oTail->next = nullptr;
  if (eHead == nullptr) return oHead;

  eTail->next = oHead;

  return eHead;
}
