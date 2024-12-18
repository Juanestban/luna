#ifndef LUNA_VM_DEBUG_H
#define LUNA_VM_DEBUG_H

#include <cstdio>

#include "chunk.h"

namespace Luna::VM {

void disassemble_chunk(Chunk *chunk, const char *name);

int disassemble_instruction(Chunk *chunk, int offset);

} // namespace Luna::VM

#endif
