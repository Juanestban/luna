#include "chunk.h"

namespace Luna::VM {

void Chunk::write_me(uint8_t byte) {
  if (capacity < count + 1) {
    int old_capacity = this->capacity;
    this->capacity = GROW_CAPACITY(old_capacity);
    this->code = GROW_ARRAY(uint8_t, this->code, old_capacity, this->capacity);
  }

  this->code[this->count] = byte;
  this->count++;
}

void Chunk::free_me() {
  FREE_ARRAY(uint8_t, this->code, this->capacity);

  this->init_me();
}

} // namespace Luna::VM
