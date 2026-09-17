#pragma once
#include <cstddef>
#include <iostream>
#include <stdexcept>

template <typename T>
class LinkedList
{
private:
  class Node
  {
  public:
    T data;
    Node* next;

    Node() : data{}, next(nullptr) {}
    Node(const T& data, Node* next = nullptr) : data(data), next(next) {}
  };

  Node* dummy;
  std::size_t size_;

public:
  LinkedList() : dummy(new Node), size_(0) {}

  ~LinkedList()
  {
    Node* head = dummy->next;
    while (head)
    {
      Node* next = head->next;
      delete head;
      head = next;
    }
    delete dummy;
  }

  void insert(std::size_t index, const T& data)
  {
    if (index > size_) throw std::out_of_range("Index out of range");
    Node* prev = dummy;

    for (std::size_t i = 0; i < index; ++i) prev = prev->next;

    prev->next = new Node(data, prev->next);
    ++size_;
  }

  void push_front(const T& data)
  {
    this->insert(0, data);
  }

  void push_back(const T& data)
  {
    this->insert(this->size_, data);
  }

  void remove(std::size_t index)
  {
    if (index >= this->size_) throw std::out_of_range("Index out of range");

    Node* prev = dummy;
    for (std::size_t i = 0; i < index; ++i) prev = prev->next;

    Node* removedNode = prev->next;
    prev->next = removedNode->next;
    delete removedNode;
    --this->size_;
  }

  void pop_front()
  {
    if (this->empty()) throw std::underflow_error("Cannot pop from an empty LinkedList");
    this->remove(0);
  }

  void pop_back()
  {
    if (this->empty()) throw std::underflow_error("Cannot pop from an empty LinkedList");
    this->remove(this->size_ - 1);
  }

  std::size_t size() const
  {
    return this->size_;
  }

  bool empty() const
  {
    return this->size_ == 0;
  }

  void print() const
  {
    for (Node* head = dummy->next; head != nullptr; head = head->next)
      std::cout << head->data << " -> ";
    std::cout << "nullptr\n";
  }
};
