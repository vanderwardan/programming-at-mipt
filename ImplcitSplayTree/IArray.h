#pragma once

#include <vector>

class IArray {
 public:
  virtual int getSum(int left, int right) = 0;
  virtual void insert(int value, int pos) = 0;
  virtual void assign(int value, int pos) = 0;
  virtual void add(int value, int left, int right) = 0;
  virtual void nextPermutation(int left, int right) = 0;
  virtual void print(std::vector<int> *v) = 0;
};