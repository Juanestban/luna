#include "value.h"
#include "memory.h"

namespace Luna::VM {

void ValueArray::print_me(Value value) { printf("%g", value); }

void ValueArray::init_me() {
  this->values = nullptr;
  this->capacity = 0;
  this->count = 0;
}

void ValueArray::write_me(Value value) {
  if (this->capacity < this->count + 1) {
    int old_capacity = this->capacity;
    this->capacity = GROW_CAPACITY(old_capacity);
    this->values =
        GROW_ARRAY(Value, this->values, old_capacity, this->capacity);
  }

  this->values[this->count] = value;
  this->count++;
}

void ValueArray::free_me() {
  FREE_ARRAY(Value, this->values, this->capacity);

  this->init_me();
}

} // namespace Luna::VM
