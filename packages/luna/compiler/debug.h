#ifndef LUNA_COMPILER_DEBUG_H
#define LUNA_COMPILER_DEBUG_H

#include <cstdio>

#include "chunk.h"

namespace Luna::Compiler {

void disassemble_chunk(Chunk *chunk, const char *name);

int disassemble_instruction(Chunk *chunk, int offset);

} // namespace Luna::Compiler

#endif
