#pragma once

struct Node {
  Node() : left(nullptr), right(nullptr), parent(nullptr), key(),
           size(1), sum(), add(), max_suff(1),
           max_pref(1), last(key), first(key),
           isReverse(false) {
  }
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

//for safe work with node
int nodeSize(const Node *node);

int nodeSum(const Node *node);

int nodeMaxSuff(const Node *node);      //len of max suffix

int nodeMaxPref(const Node *node);      //len of max preffix

int nodeFirst(const Node *node);        //first element in subtree

int nodeLast(const Node *node);         //last element in subtree
