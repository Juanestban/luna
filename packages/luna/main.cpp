#include "vm/chunk.h"
#include "vm/debug.h"

using namespace Luna::VM;

int main() {
  Chunk *chunk = new Chunk();

  chunk->write_me(RETURN);
  disassemble_chunk(chunk, "test chunk");

  chunk->free_me();

  return 0;
}
