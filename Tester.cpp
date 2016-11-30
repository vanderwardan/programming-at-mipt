
#include "ImplicitSplayTree.h"
#include <fstream>
#include <time.h>

class VectorRealise : public IArray {
 public:
  int getSum(int left, int right);

  void insert(int value, int pos);

  void assign(int value, int pos);

  void add(int value, int left, int right);

  void nextPermutation(int left, int right);

  void print(std::vector<int> *v);

 private:
  std::vector<int> array;
};

bool test(IArray *s1, IArray *s2) {
  std::ifstream input("input.txt");

  int n;
  input >> n;

  std::vector<int> v1, v2;

  for (size_t i = 0; i < n; i++) {
    v1.clear();
    v2.clear();

    int command, value, left, right;
    input >> command >> value >> left >> right;
    std::cout << "Test #" << i + 1 << ": ";

    switch (command) {
      case 0: {
        v1.push_back(s1->getSum(left, right));
        v2.push_back(s2->getSum(left, right));
        break;
      }
      case 1: {
        s1->insert(value, left);
        s2->insert(value, left);
        break;
      }
      case 2: {
        s1->assign(value, left);
        s2->assign(value, left);
        break;
      }
      case 3: {
        s1->add(value, left, right);
        s2->add(value, left, right);
        break;
      }
      case 4: {
        s1->nextPermutation(left, right);
        s2->nextPermutation(left, right);
        break;
      }
    }

    s1->print(&v1);
    s2->print(&v2);

    if (v1 != v2) {
      input.close();
      return false;
    }

    std::cout << "OK" << "\n";
  }

  input.close();
  return true;
}

void generateTests() {
  std::ofstream out("input.txt");

  int size = 1;
  int n = 10000;
  srand(time(NULL));

  out << n + 1 << "\n";
  out << 1 << " " << 0 << " " << 0 << " " << 0 << "\n";

  for (size_t i = 0; i < n; i++) {
    int command = rand() % 8;
    if (command > 4) command = 1;
    int left = rand() % size;
    int right = left + rand() % (size - left);
    int value = rand() % 100;
    if (command == 1) size++;
    out << command << " " << value << " " << left << " " << right << "\n";
  }

  out.close();
}

int main() {
  ImplicitSplayTree tree1;
  VectorRealise vector1;

  generateTests();

  if (test(&tree1, &vector1)) std::cout << "All tests have passed successfully\n";
  else std::cout << "Tests have failed\n";

  return 0;
}

//--------------VectorRealise----------------------------------------
int VectorRealise::getSum(int left, int right) {
  return std::accumulate(array.begin() + left, array.begin() + right + 1, 0);
}

void VectorRealise::insert(int value, int pos) {
  array.insert(array.begin() + pos, value);
}

void VectorRealise::assign(int value, int pos) {
  array[pos] = value;
}

void VectorRealise::add(int value, int left, int right) {
  std::for_each(array.begin() + left, array.begin() + right + 1, [value](int &n) { n += value; });
}

void VectorRealise::nextPermutation(int left, int right) {
  std::next_permutation(array.begin() + left, array.begin() + right + 1);
}

void VectorRealise::print(std::vector<int> *v) {
  for (int i = 0; i < array.size(); i++) {
    v->push_back(array[i]);
  }
}
//------------------------------------------------------------------------
