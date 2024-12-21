#ifndef LUNA_VM_H
#define LUNA_VM_H

#include <cstdio>

#include "../compiler/chunk.h"
#include "../compiler/compiler.h"
#include "../compiler/debug.h"
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
  VM();

public:
  void init_me();
  void free_me();
  void reset_stack();

public:
  InterpertResult run();
  InterpertResult interpret(Chunk *chunk);
  InterpertResult interpret(const char *source);

  void push(Value value);
  Value pop();
  Value peek(int distance);

  bool is_falsey(Value value);

  void runtime_error(const char *format, ...);
};

} // namespace Luna::VM

#endif
