/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   AVL.h
 * Author: LTSACH
 *
 * Created on 12 September 2020, 17:11
 */

#ifndef AVL_H
#define AVL_H
#include "BST.h"
#include "support/DLinkedList.h"
#include "support/Queue.h"

#define XNode typename BST<K, V>::Node

template <class K, class V>
class AVL : public BST<K, V>
{
public:
  void add(K key, V value = 0)
  {
    // TODO Q1b: insert a new key, ignore duplicates, update count.
    bool exists = false;
    this->search(key, exists);
    if (exists) return;
    this->pRoot = add(this->pRoot, new XNode(typename BST<K, V>::Entry(key, value)));
    this->count++;
  }

  V remove(K key, bool* success = 0)
  {
    V retValue = 0;  // O: NULL
    bool success_ = false;
    this->pRoot = remove(this->pRoot, key, success_, retValue);
    if (success_) this->count -= 1;
    // if(success != 0) if (success != 0) *success = success_;
    if (success != 0) *success = success_;
    return retValue;
  }
  int height()
  {
    if (this->pRoot == 0) return 0;
    return this->pRoot->height();
  }
  void println(string (*entry2str)(K&, V&) = 0)
  {
    cout << "Tree height: " << this->height() << endl;
    cout << "Tree nodes:" << endl;
    cout << this->toString(entry2str, true) << endl;
  }
  /*
   * bfactor: designed for being used to test AVL tree
   *          so, maybe not useful for real application
   * It returns a list of balance factor (string) of nodes visited with BFS-traversal
   * (see AVLTest for example)
   */
  DLinkedList<string> bfactor()
  {
    DLinkedList<string> list;
    if (this->pRoot == 0) return list;

    Queue<XNode*> queue;
    queue.push(this->pRoot);
    while (!queue.empty())
    {
      XNode* pNode = queue.pop();
      if (pNode->bfactor == nEMPTY)
        list.add("EMPTY");
      else if (pNode->bfactor == nLH)
        list.add("LH");
      else if (pNode->bfactor == nEH)
        list.add("EH");
      else
        list.add("RH");

      if (pNode->bfactor == nEMPTY)
      {
        delete pNode;  // because alloc in [1] or [2]
        continue;
      }
      if ((pNode->pLeft == 0) && (pNode->pRight == 0)) continue;

      if (pNode->pLeft != 0)
        queue.push(pNode->pLeft);
      else
        queue.push(new XNode());  //[1]
      if (pNode->pRight != 0)
        queue.push(pNode->pRight);
      else
        queue.push(new XNode());  //[2]
    }
    return list;
  }

private:
  // TODO Q1b: implement rotations, rebalancing, and recursive updates.
  // Node::updateHeight() and Node::bfactor are supplied infrastructure.
  XNode* rotateLeft(XNode* root)
  {
    XNode* newRoot = root->pRight;
    root->pRight = newRoot->pLeft;
    newRoot->pLeft = root;

    root->updateHeight();
    newRoot->updateHeight();

    return newRoot;
  }
  XNode* rotateRight(XNode* root)
  {
    XNode* newRoot = root->pLeft;
    root->pLeft = newRoot->pRight;
    newRoot->pRight = root;

    root->updateHeight();
    newRoot->updateHeight();

    return newRoot;
  }
  XNode* rebalance(XNode* root)
  {
    if (!root) return nullptr;
    root->updateHeight();

    if (root->bfactor < -1)  // Left heavy
    {
      if (root->pLeft->bfactor > 0) root->pLeft = rotateLeft(root->pLeft);
      return rotateRight(root);
    }

    if (root->bfactor > 1)  // Right heavy
    {
      if (root->pRight->bfactor < 0) root->pRight = rotateRight(root->pRight);
      return rotateLeft(root);
    }

    return root;
  }
  XNode* add(XNode* root, XNode* newNode)
  {
    if (!root) return newNode;
    if (newNode->data.key < root->data.key)
      root->pLeft = add(root->pLeft, newNode);
    else if (newNode->data.key > root->data.key)
      root->pRight = add(root->pRight, newNode);

    return rebalance(root);
  }
  XNode* remove(XNode* root, K key, bool& success, V& retValue)
  {
    if (!root)
    {
      success = false;
      return 0;
    }

    if (key < root->data.key)
    {
      root->pLeft = remove(root->pLeft, key, success, retValue);
      return success ? rebalance(root) : root;
    }
    if (root->data.key < key)
    {
      root->pRight = remove(root->pRight, key, success, retValue);
      return success ? rebalance(root) : root;
    }

    success = true;
    retValue = root->data.value;

    XNode *left = root->pLeft, *right = root->pRight;
    if (!left || !right)
    {
      XNode* child = left ? left : right;
      root->data.value = 0;
      delete root;
      return child;
    }

    XNode* succ = right;
    while (succ->pLeft != 0) succ = succ->pLeft;
    K succKey = succ->data.key;
    root->data = succ->data;

    bool dummyOk = false;
    V dummyValue = 0;
    root->pRight = remove(root->pRight, succKey, dummyOk, dummyValue);
    return rebalance(root);
  }
};

#endif /* AVL_H */
