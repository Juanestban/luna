#ifndef LUNA_COMPILER_H
#define LUNA_COMPILER_H

#include "chunk.h"
#include "lexer/parser.h"
#include "lexer/scanner.h"


namespace Luna::Compiler {

bool compile(const char *source, Chunk *chunk);

}

#endif
