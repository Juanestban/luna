#include "debug.h"
#include "chunk.h"
#include "value.h"

namespace Luna::VM {

void disassemble_chunk(Chunk *chunk, const char *name) {
  printf("== %s ==\n", name);

  for (int offset = 0; offset < chunk->count;) {
    offset = disassemble_instruction(chunk, offset);
  }
}

static int simple_instruction(const char *name, int offset) {
  printf("%s\n", name);

  return offset + 1;
}

static int constant_instruction(const char *name, Chunk *chunk, int offset) {
  uint8_t constant = chunk->code[offset + 1];

  printf("%-16s %4d '", name, constant);

  chunk->constants.print_me(chunk->constants.values[constant]);

  printf("'\n");

  return offset + 2;
}

int disassemble_instruction(Chunk *chunk, int offset) {
  printf("%04d ", offset);

  uint8_t instruction = chunk->code[offset];

  switch (instruction) {
  case CONSTANT:
    return constant_instruction("CONSTANT", chunk, offset);
  case RETURN:
    return simple_instruction("RETURN", offset);
  default:
    printf("Unknown opcode %d\n", instruction);

    return offset + 1;
  }
}

} // namespace Luna::VM
