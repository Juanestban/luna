#ifndef LUNA_VM_CHUNK_H
#define LUNA_VM_CHUNK_H

#include "common.h"
#include "memory.h"
#include "value.h"

namespace Luna::VM {

enum OpCode {
  RETURN,
  CONSTANT,
};

class Chunk {
public:
  int count;
  int capacity;
  uint8_t *code;
  ValueArray constants;

public:
  Chunk() : count(0), capacity(0), code(nullptr) {};

  void init_me() {
    this->count = 0;
    this->capacity = 0;
    this->code = nullptr;
    this->constants = ValueArray();
  }

  int add_constant(Value value);

  void write_me(uint8_t byte);

  void free_me();
};

}; // namespace Luna::VM

#endif
