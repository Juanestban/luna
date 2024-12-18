#include "chunk.h"
#include "memory.h"

namespace Luna::VM {

int Chunk::add_constant(Value value) {
  this->constants.write_me(value);

  return this->constants.count - 1;
}

void Chunk::write_me(uint8_t byte, int line) {
  if (capacity < count + 1) {
    int old_capacity = this->capacity;
    this->capacity = GROW_CAPACITY(old_capacity);
    this->code = GROW_ARRAY(uint8_t, this->code, old_capacity, this->capacity);
    this->lines = GROW_ARRAY(int, this->lines, old_capacity, this->capacity);
  }

  this->code[this->count] = byte;
  this->lines[this->count] = line;
  this->count++;
}

void Chunk::free_me() {
  FREE_ARRAY(uint8_t, this->code, this->capacity);
  FREE_ARRAY(int, this->lines, this->capacity);
  this->constants.free_me();

  this->init_me();
}

} // namespace Luna::VM
