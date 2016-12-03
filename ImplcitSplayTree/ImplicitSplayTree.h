#pragma once

#include <iostream>
#include <algorithm>
#include <numeric>
#include <assert.h>
#include <queue>

#include "Node.h"

class IArray {
 public:
  virtual int getSum(int left, int right) = 0;
  virtual void insert(int value, int pos) = 0;
  virtual void assign(int value, int pos) = 0;
  virtual void add(int value, int left, int right) = 0;
  virtual void nextPermutation(int left, int right) = 0;
  virtual void print(std::vector<int> *v) = 0;
};

//IArray implementation
class ImplicitSplayTree : public IArray {
 public:
  ImplicitSplayTree() : root_(nullptr) {}

  ~ImplicitSplayTree();

  int getSum(int left, int right);

  void insert(int value, int pos);

  void assign(int value, int pos);

  void add(int value, int left, int right);

  void nextPermutation(int left, int right);

  void print(std::vector<int> *v);

 private:

  enum side { LEFT, RIGHT };

  Node *merge(Node *left, Node *right);

  std::pair<Node *, Node *> split(Node *tree, int pos); //split elements from 0 to pos,
  //or nullptr, if pos==-1

  void splay(Node *&tree, Node *node);

  void assign(Node *&tree, int value, int pos);

  void rotate(Node *node, side side);        //side response for type of rotation

  //for work with subtree parts
  Node *&getChild(Node *node, side side) const;

  int &getPart(Node *node, side side) const; //max suffix or prefix, depending on side

  int maxPart(const Node *node, side side) const;  //len of max suffix or prefix, depending on side

  int element(const Node *node, side side) const;  //first or last element in subtree, depending on side

  //for delayed modification
  void update(Node *node);                //update all parametrs of node, if it childs up to date

  void updatePart(Node *node, side side);    //side response for the child

  void push(Node *node);                  //it's for delayed modification

  //for tests
  void print(Node *node, std::vector<int> *v);

  Node *root_;
};
