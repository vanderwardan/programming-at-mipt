#include "ImplicitSplayTree.h"

ImplicitSplayTree::~ImplicitSplayTree() {
  std::queue < Node * > queue;
  queue.push(root_);

  while (!queue.empty()) {
    Node *tmp = queue.front();
    queue.pop();
    if (tmp != nullptr) {
      queue.push(tmp->right);
      queue.push(tmp->left);
      delete tmp;
    }
  }
}

//main functions
int ImplicitSplayTree::getSum(int left, int right) {
  assert(left >= 0 && left <= right && right < nodeSize(root_));

  std::pair < Node * , Node * > p1 = split(root_, left - 1);
  std::pair < Node * , Node * > p2 = split(p1.second, right - left);

  int res = nodeSum(p2.first);
  p1.second = merge(p2.first, p2.second);
  root_ = merge(p1.first, p1.second);
  return res;
}

void ImplicitSplayTree::insert(int value, int pos) {
  assert(pos >= 0 && pos <= nodeSize(root_));

  Node *newNode = new Node(value);

  if (root_ == nullptr) {
    root_ = newNode;
  } else {
    std::pair < Node * , Node * > p = split(root_, pos - 1);
    p.first = merge(p.first, newNode);
    root_ = merge(p.first, p.second);
  }
}

void ImplicitSplayTree::assign(int value, int pos) {
  assign(root_, value, pos);
}

void ImplicitSplayTree::add(int value, int left, int right) {
  assert(left >= 0 && left <= right && right < nodeSize(root_));

  std::pair < Node * , Node * > p1 = split(root_, left - 1);
  std::pair < Node * , Node * > p2 = split(p1.second, right - left);

  p2.first->add += value;

  p1.second = merge(p2.first, p2.second);
  root_ = merge(p1.first, p1.second);
}

void ImplicitSplayTree::nextPermutation(int left, int right) {
  assert(left >= 0 && left <= right && right < nodeSize(root_));

  std::pair < Node * , Node * > p1 = split(root_, left - 1);
  std::pair < Node * , Node * > p2 = split(p1.second, right - left);

  //p2.first is subtree we should to work with
  Node *node = p2.first;
  push(node);
  if (nodeMaxSuff(node) == nodeSize(node)) {
    node->isReverse = !node->isReverse;
  } else {
    //find max decreasing suf
    int pos = node->size - nodeMaxSuff(node);
    std::pair < Node * , Node * > nodep = split(node, pos - 1);
    int key = nodeLast(nodep.first);

    //find node in suff for swap
    Node *swapNode = nodep.second;//node in suff for swap
    int swapPos = nodeSize(swapNode->left);//this node pos
    push(swapNode);

    while (true) {
      if (swapNode->key > key && swapNode->right != nullptr) {
        swapNode = swapNode->right;
        push(swapNode);
        swapPos += nodeSize(swapNode->left) + 1;
      } else if (swapNode->key <= key && swapNode->left != nullptr) {
        swapNode = swapNode->left;
        push(swapNode);
        swapPos -= nodeSize(swapNode->right) + 1;
      } else break;
    }

    if (swapNode->key <= key) {
      if (swapNode->parent != nullptr) {//the case, when last while doesn't exec
        int tmp = swapPos;
        while (tmp != swapPos - 1) {
          if (swapNode->parent->right == swapNode) {
            tmp -= nodeSize(swapNode->left) + 1;
          } else {
            tmp += nodeSize(swapNode->right) + 1;
          }
          swapNode = swapNode->parent;
        }
      } else {
        swapNode = swapNode->left;
        assert(swapNode != nullptr);
        while (swapNode->right != nullptr) swapNode = swapNode->right;
      }
      swapPos--;
    }

    //swap last element in nodep.first with swapNode
    assign(nodep.first, swapNode->key, pos - 1);
    assign(nodep.second, key, swapPos);

    //reverse suff
    nodep.second->isReverse = !nodep.second->isReverse;
    node = merge(nodep.first, nodep.second);
  }

  p2.first = node;
  p1.second = merge(p2.first, p2.second);
  root_ = merge(p1.first, p1.second);
}

void ImplicitSplayTree::print(std::vector<int> *v) {
  print(root_, v);
}

//---------------
Node *ImplicitSplayTree::merge(Node *left, Node *right) {
  if (right == nullptr) return left;
  if (left == nullptr) return right;

  push(left);

  if (left->right != nullptr) {
    Node *curNode = left;
    while (curNode->right != nullptr) {
      curNode = curNode->right;
      push(curNode);
    }
    splay(left, curNode);
  }

  push(left);
  left->right = right;
  right->parent = left;
  update(left);

  return left;
}

std::pair<Node *, Node *> ImplicitSplayTree::split(Node *tree, int pos) {
  assert(tree != nullptr);
  assert(nodeSize(tree) > pos && pos >= -1);

  if (nodeSize(tree) == pos + 1) {
    return std::make_pair(tree, nullptr);
  }

  if (pos == -1) {
    return std::make_pair(nullptr, tree);
  }

  Node *curNode = tree;
  push(curNode);

  int curpos = nodeSize(curNode->left);
  while (curpos != pos) {
    assert(curNode != nullptr);
    if (curpos < pos) {
      curNode = curNode->right;
      push(curNode);
      curpos += nodeSize(curNode->left) + 1;
    } else {
      curNode = curNode->left;
      push(curNode);
      curpos -= nodeSize(curNode->right) + 1;
    }
  }

  splay(tree, curNode);

  std::pair < Node * , Node * > result = std::make_pair(tree, tree->right);
  result.first->right = nullptr;
  update(result.first);
  result.second->parent = nullptr;

  return result;
}

void ImplicitSplayTree::splay(Node *&tree, Node *node) {
  if (node == nullptr) return;
  if (node == tree) return;

  while (true) {
    Node *parent = node->parent;

    //Zig
    if (parent == tree) {
      if (parent->left == node) {
        rotate(node, RIGHT);
      } else {
        rotate(node, LEFT);
      }
      if (parent == tree) break;
      continue;
    }

    Node *grandpa = parent->parent;
    //Zig-Zig
    if (parent != nullptr && (grandpa->left == parent && parent->left == node)
        || (grandpa->right == parent && parent->right == node)) {
      if (grandpa->left == parent && parent->left == node) {
        rotate(parent, RIGHT);
        rotate(node, RIGHT);
      } else {
        rotate(parent, LEFT);
        rotate(node, LEFT);
      }
      if (grandpa == tree) break;
      continue;
    }

    //Zig-Zag
    if (parent != nullptr && (grandpa->left == parent && parent->right == node)
        || (grandpa->right == parent && parent->left == node)) {
      if (grandpa->left == parent && parent->right == node) {
        rotate(node, LEFT);
        rotate(node, RIGHT);
      } else {
        rotate(node, RIGHT);
        rotate(node, LEFT);
      }
      if (grandpa == tree) break;
      continue;
    }
  }
  tree = node;
}

void ImplicitSplayTree::assign(Node *&tree, int value, int pos) {
  assert(pos >= 0 && pos < nodeSize(tree));

  Node *curNode = tree;

  int curpos = nodeSize(curNode->left);

  while (curpos != pos) {
    assert(curNode != nullptr);
    if (curpos < pos) {
      curNode = curNode->right;
      push(curNode);
      curpos += nodeSize(curNode->left) + 1;
    } else {
      curNode = curNode->left;
      push(curNode);
      curpos -= nodeSize(curNode->right) + 1;
    }
  }

  splay(tree, curNode);
  tree->key = value;//tree is curNode now
  update(tree);
}

void ImplicitSplayTree::rotate(Node *a, side s1) {
  assert(a != nullptr);
  Node *b = a->parent;
  assert(b != nullptr);
  push(b);
  push(a);

  //identify kind of rotation
  side s2;
  if (s1 == RIGHT) s2 = LEFT;
  else s2 = RIGHT;

  //childs swap
  getChild(b, s2) = getChild(a, s1);
  if (getChild(a, s1) != nullptr) getChild(a, s1)->parent = b;
  getChild(a, s1) = b;

  //parents swap
  a->parent = b->parent;
  b->parent = a;
  if (a->parent != nullptr) {
    if (getChild(a->parent, s2) == b) getChild(a->parent, s2) = a;
    else getChild(a->parent, s1) = a;
  }

  update(b);
  update(a);
}

//for work with subtree parts
Node *&ImplicitSplayTree::getChild(Node *node, side s) const {
  assert(node != nullptr);
  if (s == LEFT) {
    return node->left;
  } else {
    return node->right;
  }
}

int &ImplicitSplayTree::getPart(Node *node, side s) const {
  assert(node != nullptr);
  if (s == LEFT) {
    return node->max_pref;
  } else {
    return node->max_suff;
  }
}

int ImplicitSplayTree::maxPart(const Node *node, side s) const {
  //assert(node != nullptr);
  if (node == nullptr) return 0;
  if (s == LEFT) {
    return nodeMaxPref(node);
  } else {
    return nodeMaxSuff(node);
  }
}

int ImplicitSplayTree::element(const Node *node, side s) const {
  if (node == nullptr) return 0;
  if (s == LEFT) {
    return nodeFirst(node);
  } else {
    return nodeLast(node);
  }
}

//for delayed modification
void ImplicitSplayTree::update(Node *node) {
  if (node == nullptr) return;

  node->size = nodeSize(node->left) + nodeSize(node->right) + 1;
  node->sum = nodeSum(node->left) + nodeSum(node->right) + node->key;

  updatePart(node, RIGHT);
  updatePart(node, LEFT);

  if (node->right != nullptr) {
    node->last = nodeLast(node->right);
  } else {
    node->last = node->key;
  }

  if (node->left != nullptr) {
    node->first = nodeFirst(node->left);
  } else {
    node->first = node->key;
  }
}

void ImplicitSplayTree::updatePart(Node *node, side s1) {
  side s2;
  if (s1 == RIGHT) {//for suff
    s2 = LEFT;
  } else {          //for pref
    s2 = RIGHT;
  }

  getPart(node, s1) = maxPart(getChild(node, s1), s1);
  if (maxPart(getChild(node, s1), s1) == nodeSize(getChild(node, s1))) {
    if (getChild(node, s1) == nullptr || node->key >= element(getChild(node, s1), s2)) {
      getPart(node, s1)++;
      if (getChild(node, s2) != nullptr && element(getChild(node, s2), s1) >= node->key) {
        getPart(node, s1) += maxPart(getChild(node, s2), s1);
      }
    }
  }
}

void ImplicitSplayTree::push(Node *node) {
  if (node == nullptr) return;
  if (node->add != 0) {

    node->key += node->add;
    node->first += node->add;
    node->last += node->add;
    node->sum += node->add * node->size;

    if (node->left != nullptr) node->left->add += node->add;
    if (node->right != nullptr) node->right->add += node->add;

    node->add = 0;
  }
  if (node->isReverse) {
    std::swap(node->left, node->right);

    if (node->left != nullptr) node->left->isReverse = !node->left->isReverse;
    if (node->right != nullptr) node->right->isReverse = !node->right->isReverse;

    std::swap(node->first, node->last);
    std::swap(node->max_pref, node->max_suff);

    node->isReverse = 0;
  }
}

//for tests
void ImplicitSplayTree::print(Node *node, std::vector<int> *v) {
  if (node == nullptr) return;
  push(node);
  print(node->left, v);
  v->push_back(node->key);
  print(node->right, v);
}
