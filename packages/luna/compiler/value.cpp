#include "value.h"
#include <cstdio>

namespace Luna::Compiler {

bool ValueArray::values_equal(Value a, Value b) {
  if (a.type != b.type) {
    return false;
  }

  switch (a.type) {
  case VAL_BOOL: {
    bool as_bool_a = AS_BOOL(a);
    bool as_bool_b = AS_BOOL(b);

    return as_bool_a == as_bool_b;
  }
  case VAL_NULL: {
    return true;
  }
  case VAL_NUMBER: {
    double as_number_a = AS_NUMBER(a);
    double as_number_b = AS_NUMBER(b);

    return as_number_a == as_number_b;
  }
  default:
    return false; // Unreachable
  }
}

void ValueArray::print_me(Value value) {
  switch (value.type) {
  case VAL_BOOL: {
    bool boolean = AS_BOOL(value);
    printf(boolean ? "true" : "false");
    break;
  }
  case VAL_NULL: {
    printf("null");
    break;
  }
  case VAL_NUMBER: {
    double number = AS_NUMBER(value);
    printf("%g", number);
  }
  case VAL_INT:
  case VAL_FLOAT:
  case VAL_OBJ:
    break;
  }
}

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

} // namespace Luna::Compiler
