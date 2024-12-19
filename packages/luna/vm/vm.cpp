#include "vm.h"

namespace Luna::VM {

void VM::init_me() { this->reset_stack(); }

void VM::free_me() {}

void VM::reset_stack() { this->stack_top = this->stack; }

InterpertResult VM::run() {
#define READ_BYTE() (*this->ip++)
#define READ_CONSTANT() (this->chunk->constants.values[READ_BYTE()])
#define BINARY_OP(op)                                                          \
  do {                                                                         \
    double b = this->pop();                                                    \
    double a = this->pop();                                                    \
    this->push(a op b);                                                        \
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
    case OP_ADD:
      BINARY_OP(+);
      break;
    case OP_SUBTRACT:
      BINARY_OP(-);
      break;
    case OP_MULTIPLY:
      BINARY_OP(*);
      break;
    case OP_DIVIDE:
      BINARY_OP(/);
      break;
    case OP_NEGATE:
      this->push(-this->pop());
      break;
    case OP_RETURN: {
      ValueArray::print_me(this->pop());
      printf("\n");

      return INTERPRET_OK;
    }
    default:
      return INTERPRET_OK;
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

  if (!compile(source, chunk)) {
    chunk->free_me();

    return INTERPRET_COMPILE_ERROR;
  }

  this->chunk = chunk;
  this->ip = this->chunk->code;

  InterpertResult result = this->run();

  chunk->free_me();

  return result;
};

} // namespace Luna::VM
