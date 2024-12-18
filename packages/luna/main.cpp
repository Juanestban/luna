#include "vm/chunk.h"
#include "vm/debug.h"

using namespace Luna::VM;

int main() {
  Chunk *chunk = new Chunk();
  int constant = chunk->add_constant(1.2);

  chunk->write_me(CONSTANT, 123);
  chunk->write_me(constant, 123);

  chunk->write_me(RETURN, 123);
  disassemble_chunk(chunk, "test chunk");

  chunk->free_me();

  return 0;
}
