#include "compiler/chunk.h"
#include "compiler/debug.h"
#include "vm/vm.h"

using namespace Luna::Compiler;
using namespace Luna::VM;

int main() {
  VM *vm = new VM();

  Chunk *chunk = new Chunk();
  int constant = chunk->add_constant(1.2);

  chunk->write_me(OP_CONSTANT, 123);
  chunk->write_me(constant, 123);

  constant = chunk->add_constant(3.4);
  chunk->write_me(OP_CONSTANT, 123);
  chunk->write_me(constant, 123);

  chunk->write_me(OP_ADD, 123);

  constant = chunk->add_constant(5.6);
  chunk->write_me(OP_CONSTANT, 123);
  chunk->write_me(constant, 123);

  chunk->write_me(OP_DIVIDE, 123);
  chunk->write_me(OP_NEGATE, 123);

  chunk->write_me(OP_RETURN, 123);
  disassemble_chunk(chunk, "test chunk");
  vm->interpret(chunk);

  vm->free_me();
  chunk->free_me();

  return 0;
}
