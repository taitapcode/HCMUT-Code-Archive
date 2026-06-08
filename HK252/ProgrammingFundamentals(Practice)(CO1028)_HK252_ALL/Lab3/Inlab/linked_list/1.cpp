#include "linked_list.h"

Node *createLinkedList(int n)
{
  int data;
  Node *head = nullptr, *tail = nullptr;
  while(n--)
  {
    cin >> data;
    Node *newNode = new Node();
    newNode->data = data;
    if (head == nullptr) head = tail = newNode;
    else tail = tail->next = newNode;
  }

  return head;
}
