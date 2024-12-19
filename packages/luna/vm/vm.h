#ifndef LUNA_VM_H
#define LUNA_VM_H

#include <cstdio>

#include "../compiler/chunk.h"
#include "../compiler/compiler.h"
#include "../compiler/value.h"

namespace Luna::VM {

#define STACK_MAX 256

using namespace Compiler;

enum InterpertResult {
  INTERPRET_OK,
  INTERPRET_COMPILE_ERROR,
  INTERPRET_RUNTIME_ERROR
};

class VM {
public:
  Chunk *chunk;
  uint8_t *ip;
  Value stack[STACK_MAX];
  Value *stack_top;

public:
  VM() { this->reset_stack(); }

  void init_me();
  void free_me();
  void reset_stack();

  InterpertResult run();
  InterpertResult interpret(Chunk *chunk);
  InterpertResult interpret(const char *source);

  void push(Value value);
  Value pop();
};

} // namespace Luna::VM

#endif
