#include "Node.h"

//for safe work with node
int nodeSize(const Node *node) {
  if (node == nullptr) return 0;
  else return node->size;
}

int nodeSum(const Node *node) {
  if (node != nullptr) {
    return node->sum + node->add * node->size;
  } else {
    return 0;
  }
}

int nodeMaxSuff(const Node *node) {
  if (node != nullptr) {
    return (!node->isReverse) ? node->max_suff : node->max_pref;
  } else {
    return 0;
  }
}

int nodeMaxPref(const Node *node) {
  if (node != nullptr) {
    return (!node->isReverse) ? node->max_pref : node->max_suff;
  } else {
    return 0;
  }
}

int nodeFirst(const Node *node) {
  if (node != nullptr) {
    return ((!node->isReverse) ? node->first : node->last) + node->add;
  } else {
    return 0;
  }
}

int nodeLast(const Node *node) {
  if (node != nullptr) {
    return ((!node->isReverse) ? node->last : node->first) + node->add;
  } else {
    return 0;
  }
}