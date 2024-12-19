#ifndef LUNA_COMPILER_VALUE_H
#define LUNA_COMPILER_VALUE_H

#include <cstdio>

#include "common.h"
#include "memory.h"

namespace Luna::Compiler {

typedef double Value;

class ValueArray {
public:
  int capacity;
  int count;
  Value *values;

public:
  ValueArray() : capacity(0), count(0), values(nullptr) {}

  void init_me();

  void write_me(Value value);

  void free_me();

  static void print_me(Value value);
};

} // namespace Luna::Compiler

#endif
