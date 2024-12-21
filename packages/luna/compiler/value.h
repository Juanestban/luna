#ifndef LUNA_COMPILER_VALUE_H
#define LUNA_COMPILER_VALUE_H

#include <cstdio>
#include <variant>

#include "common.h"
#include "memory.h"

namespace Luna::Compiler {

enum ValueType { VAL_BOOL, VAL_NULL, VAL_INT, VAL_NUMBER, VAL_FLOAT, VAL_OBJ };

typedef struct {
  ValueType type;
  union {
    bool boolean;
    int integer;
    double number;
    float floating;
    void *object;
  } as;
} Value;

#define IS_BOOL(value) ((value).type == VAL_BOOL)
#define IS_NULL(value) ((value).type == VAL_NULL)
#define IS_INT(value) ((value).type == VAL_INT)
#define IS_NUMBER(value) ((value).type == VAL_NUMBER)
#define IS_FLOAT(value) ((value).type == VAL_FLOAT)

#define AS_BOOL(value) ((value).as.boolean);
#define AS_INT(value) ((value).as.integer);
#define AS_NUMBER(value) ((value).as.number);
#define AS_FLOATING(value) ((value).as.floating);

inline Value BOOL_VAL(bool value) {
  Value v;
  v.type = VAL_BOOL;
  v.as.boolean = value;
  return v;
}

inline Value NULL_VAL() {
  Value v;
  v.type = VAL_NULL;
  v.as.integer = 0;
  return v;
}

inline Value INT_VAL(int value) {
  Value v;
  v.type = VAL_INT;
  v.as.integer = value;
  return v;
}

inline Value NUMBER_VAL(double value) {
  Value v;
  v.type = VAL_NUMBER;
  v.as.number = value;
  return v;
}

inline Value FLOAT_VAL(float value) {
  Value v;
  v.type = VAL_FLOAT;
  v.as.floating = value;
  return v;
}

inline Value OBJ_VAL(void *value) {
  Value v;
  v.type = VAL_OBJ;
  v.as.object = value;
  return v;
}

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
  static bool values_equal(Value a, Value b);
};

} // namespace Luna::Compiler

#endif
