#ifndef CIRCULARLINKEDLIST_H
#define CIRCULARLINKEDLIST_H

#include <iostream>
#include <sstream>
#include <stdexcept>

#include "IList.h"
using namespace std;

template <class T>
class CircularLinkedList : public IList<T>
{
public:
  class Node;

protected:
  Node* head;
  Node* tail;
  int count;
  bool (*itemEqual)(T& lhs, T& rhs);
  void (*deleteUserData)(CircularLinkedList<T>*);

public:
  CircularLinkedList(
      void (*deleteUserData)(CircularLinkedList<T>*) = 0,
      bool (*itemEqual)(T&, T&) = 0)
      : head(nullptr), tail(nullptr), count(0), itemEqual(itemEqual), deleteUserData(deleteUserData) {}

  CircularLinkedList(const CircularLinkedList<T>& list)
      : head(nullptr), tail(nullptr), count(0), itemEqual(list.itemEqual), deleteUserData(list.deleteUserData)
  {
    copyFrom(list);
  }

  CircularLinkedList<T>& operator=(const CircularLinkedList<T>& list)
  {
    if (this == &list) return *this;
    removeInternalData();
    itemEqual = list.itemEqual;
    deleteUserData = list.deleteUserData;
    copyFrom(list);
    return *this;
  }

  ~CircularLinkedList()
  {
    removeInternalData();
  }

  void add(T e) override
  {
    if (count == 0)
    {
      head = tail = new Node(e);
      head->next = head;
    }
    else
      tail = tail->next = new Node(e, head);

    count++;
  }

  void add(int index, T e) override
  {
    if (index < 0 || index > count) throw out_of_range("CircularLinkedList::add(index, e) index out of range");

    if (index == count) return add(e);
    if (index == 0)
    {
      Node* newNode = new Node(e, head);
      tail->next = head = newNode;
    }
    else
    {
      Node* prev = head;
      for (int i = 1; i < index; i++) prev = prev->next;

      Node* newNode = new Node(e, prev->next);
      prev->next = newNode;
    }

    count++;
  }

  T removeAt(int index) override
  {
    if (index < 0 || index >= count) throw out_of_range("CircularLinkedList::removeAt(index) index out of range");

    if (count == 1)
    {
      T removedData = head->data;
      delete head;
      head = tail = nullptr;
      count--;
      return removedData;
    }

    if (index == 0)
    {
      Node* removedNode = head;
      T removedData = removedNode->data;
      head = head->next;
      tail->next = head;
      delete removedNode;
      count--;
      return removedData;
    }

    Node* prev = head;
    for (int i = 0; i < index - 1; i++) prev = prev->next;

    Node* removedNode = prev->next;
    T removedData = prev->next->data;
    prev->next = removedNode->next;

    if (index == count - 1) tail = prev;

    delete removedNode;
    count--;
    return removedData;
  }

  bool removeItem(T item, void (*removeItemData)(T) = 0) override
  {
    int index = indexOf(item);
    if (index == -1) return false;

    T removedData = removeAt(index);
    if (removeItemData) removeItemData(removedData);

    return true;
  }

  void clear() override
  {
    removeInternalData();
  }

  T& get(int index) override
  {
    if (index < 0 || index >= count) throw out_of_range("CircularLinkedList::get(index) index out of range");
    Node* curr = head;
    for (int i = 0; i < index; i++) curr = curr->next;

    return curr->data;
  }

  int indexOf(T item) override
  {
    Node* curr = head;
    for (int i = 0; i < count; i++, curr = curr->next)
      if (equals(curr->data, item, itemEqual)) return i;
    return -1;
  }

  bool empty() override { return count == 0; }
  int size() override { return count; }
  bool contains(T item) override { return indexOf(item) >= 0; }

  string toString(string (*item2str)(T&) = 0) override
  {
    stringstream ss;
    ss << "[";
    Node* cur = head;
    for (int i = 0; i < count; ++i)
    {
      if (i > 0) ss << ", ";
      if (item2str)
        ss << item2str(cur->data);
      else
        ss << cur->data;
      cur = cur->next;
    }
    ss << "]";
    return ss.str();
  }

  void println(string (*item2str)(T&) = 0)
  {
    cout << toString(item2str) << endl;
  }

protected:
  static bool equals(T& lhs, T& rhs, bool (*itemEqual)(T&, T&))
  {
    return itemEqual ? itemEqual(lhs, rhs) : (lhs == rhs);
  }

  void copyFrom(const CircularLinkedList<T>& list)
  {
    Node* cur = list.head;
    for (int i = 0; i < list.count; ++i)
    {
      add(cur->data);
      cur = cur->next;
    }
  }

  void removeInternalData()
  {
    if (deleteUserData != 0) deleteUserData(this);
    Node* cur = head;
    for (int i = 0; i < count; ++i)
    {
      Node* next = cur->next;
      delete cur;
      cur = next;
    }
    head = tail = nullptr;
    count = 0;
  }

public:
  class Node
  {
  public:
    T data;
    Node* next;
    Node(T data, Node* next = nullptr) : data(data), next(next) {}
  };
};

#endif
