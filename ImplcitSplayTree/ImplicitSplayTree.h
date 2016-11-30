#pragma once

#include <iostream>
#include <algorithm>
#include <numeric>
#include <assert.h>
#include <queue>

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
  struct Node {
    Node() : left(nullptr), right(nullptr), parent(nullptr), key(),
             size(1), sum(), add(), max_suff(1),
             max_pref(1), last(key), first(key),
             isReverse(false) {}

    explicit Node(int key_) : Node() {
      key = key_;
      sum = key_;
      add = 0;
      last = key_;
      first = key_;
    }

    int size;       //number of nodes in subtree
    int key;

    Node *left;     //left son
    Node *right;    //right son
    Node *parent;

    //Additional information
    int sum;        //sum all nodes key in subtree
    int add;        //value to add for all elements in subtree
    int max_suff;   //length of maximal decreasing suffix
    int max_pref;   //length of minimal increasing prefix
    int last;       //last value in subtree
    int first;      //first value in subtree
    bool isReverse; //flag to reverse subtree
  };

  enum side {
    LEFT, RIGHT
  };

  Node *merge(Node *left, Node *right);

  std::pair<Node *, Node *> split(Node *tree, int pos); //split elements from 0 to pos,
  //or nullptr, if pos==-1

  void splay(Node *&tree, Node *node);

  void assign(Node *&tree, int value, int pos);

  void rotate(Node *node, side side);        //side response for type of rotation

  //for work with subtree parts
  Node *&getChild(Node *node, side side) const;

  int &getPart(Node *node, side side) const; //max suffix or prefix, depending on side

  int maxPart(Node *node, side side) const;  //len of max suffix or prefix, depending on side

  int element(Node *node, side side) const;  //first or last element in subtree, depending on side

  //for safe work with node
  int nodeSize(Node *node) const;

  int nodeSum(Node *node) const;

  int nodeMaxSuff(Node *node) const;      //len of max suffix

  int nodeMaxPref(Node *node) const;      //len of max preffix

  int nodeFirst(Node *node) const;        //first element in subtree

  int nodeLast(Node *node) const;         //last element in subtree

  //for delayed modification
  void update(Node *node);                //update all parametrs of node, if it childs up to date

  void updatePart(Node *node, side s);    //s response for the child

  void push(Node *node);                  //it's for delayed modification

  //for test
  void print(Node *node, std::vector<int> *v);

  Node *root_;
};
