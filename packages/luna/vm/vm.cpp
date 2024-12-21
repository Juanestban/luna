#include "vm.h"
#include <cstdarg>
#include <cstdio>

namespace Luna::VM {

void VM::init_me() { this->reset_stack(); }

void VM::free_me() {}

void VM::reset_stack() { this->stack_top = this->stack; }

VM::VM() { this->reset_stack(); }

Value VM::peek(int distance) { return this->stack_top[-1 - distance]; }

bool VM::is_falsey(Value value) {
  bool is_null = IS_NULL(value);
  bool as_bool = AS_BOOL(value);
  bool is_bool = IS_BOOL(value);

  return is_null || (is_bool && !as_bool);
}

InterpertResult VM::run() {
#define READ_BYTE() (*this->ip++)
#define READ_CONSTANT() (this->chunk->constants.values[READ_BYTE()])
#define BINARY_OP(valueType, op)                                               \
  do {                                                                         \
    if (!IS_NUMBER(this->peek(0)) || !IS_NUMBER(this->peek(1))) {              \
      this->runtime_error("Operands must be numbers.");                        \
      return INTERPRET_RUNTIME_ERROR;                                          \
    }                                                                          \
    double b = AS_NUMBER(this->pop());                                         \
    double a = AS_NUMBER(this->pop());                                         \
    this->push(valueType(a op b));                                             \
  } while (false)

  for (;;) {
#ifdef DEBUG_TRACE_EXECUTION
    printf("          ");

    for (Value *slot = this->stack; slot < this->stack_top; slot++) {
      printf("[ ");

      ValueArray::print_me(*slot);

      printf(" ]");
    }
    printf("\n");

    disassemble_instruction(this->chunk, (int)(this->ip - this->chunk->code));
#endif

    uint8_t instruction;

    switch (instruction = READ_BYTE()) {
    case OP_CONSTANT: {
      Value constant = READ_CONSTANT();

      this->push(constant);

      printf("\n");
      break;
    }
    case OP_NULL: {
      this->push(NULL_VAL());
      break;
    }
    case OP_TRUE: {
      this->push(BOOL_VAL(true));
      break;
    }
    case OP_FALSE: {
      this->push(BOOL_VAL(false));
      break;
    }
    case OP_EQUAL: {
      Value b = this->pop();
      Value a = this->pop();

      this->push(BOOL_VAL(ValueArray::values_equal(a, b)));
      break;
    }
    case OP_GREATER: {
      BINARY_OP(BOOL_VAL, >);
      break;
    }
    case OP_LESS: {
      BINARY_OP(BOOL_VAL, <);
      break;
    }
    case OP_ADD: {
      BINARY_OP(NUMBER_VAL, +);
      break;
    }
    case OP_SUBTRACT: {
      BINARY_OP(NUMBER_VAL, -);
      break;
    }
    case OP_MULTIPLY: {
      BINARY_OP(NUMBER_VAL, *);
      break;
    }
    case OP_DIVIDE: {
      BINARY_OP(NUMBER_VAL, /);
      break;
    }
    case OP_NOT: {
      this->push(BOOL_VAL(this->is_falsey(this->pop())));
      break;
    }
    case OP_NEGATE: {
      if (!IS_NUMBER(this->peek(0))) {
        this->runtime_error("Operand must be a number.");

        return INTERPRET_RUNTIME_ERROR;
      }
      double as_number = -AS_NUMBER(this->pop());
      Value number = NUMBER_VAL(as_number);
      this->push(number);
      break;
    }
    case OP_RETURN: {
      ValueArray::print_me(this->pop());
      printf("\n");

      return INTERPRET_OK;
    }
    default: {
      return INTERPRET_OK;
    }
    }
  }

#undef READ_BYTE
#undef READ_CONSTANT
#undef BINARY_OP
}

InterpertResult VM::interpret(Chunk *chunk) {
  this->chunk = chunk;
  this->ip = this->chunk->code;

  return this->run();
}

void VM::push(Value value) {
  *this->stack_top = value;

  this->stack_top++;
}

Value VM::pop() {
  this->stack_top--;

  return *this->stack_top;
}

InterpertResult VM::interpret(const char *source) {
  Chunk *chunk = new Chunk();

  if (!LunaCompiler::compile(source, chunk)) {
    chunk->free_me();

    return INTERPRET_COMPILE_ERROR;
  }

  this->chunk = chunk;
  this->ip = this->chunk->code;

  InterpertResult result = this->run();

  chunk->free_me();

  return result;
};

void VM::runtime_error(const char *format, ...) {
  va_list args;
  va_start(args, format);
  vfprintf(stderr, format, args);
  va_end(args);
  fputs("\n", stderr);

  size_t instruction = this->ip - this->chunk->code - 1;
  int line = this->chunk->lines[instruction];
  fprintf(stderr, "[line %d] in script\n", line);
  reset_stack();
}

} // namespace Luna::VM
