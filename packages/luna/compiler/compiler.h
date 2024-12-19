#ifndef LUNA_COMPILER_H
#define LUNA_COMPILER_H

#include <cstdint>
#include <cstdio>
#include <cstdlib>

#include "chunk.h"
#include "common.h"
#include "debug.h"
#include "lexer/parser.h"
#include "lexer/scanner.h"

namespace Luna::Compiler {

using namespace Lexer;

enum Precedence {
  PREC_NONE,
  PREC_ASSIGNMENT, // =
  PREC_OR,         // or
  PREC_AND,        // and
  PREC_EQUALITY,   // == !=
  PREC_COMPARISON, // < > <= >=
  PREC_TERM,       // + -
  PREC_FACTOR,     // * /
  PREC_UNARY,      // ! -
  PREC_CALL,       // . ()
  PREC_PRIMARY
};

typedef void (*ParseFn)();

struct ParseRule {
  ParseFn prefix;
  ParseFn infix;
  Precedence precedence;
};

class LunaCompiler {
public:
  static Scanner *scanner;
  static Parser *parser;
  static Chunk *chunk;
  static ParseRule rules[];

public:
  static bool compile(const char *source, Chunk *chunk);
  static void emit_byte(uint8_t byte);
  static void emit_bytes(uint8_t byte1, uint8_t byte2);
  static void end_compiler();
  static void emit_return();
  static void advance();
  static void expression();
  static void emit_constant(Value value);
  static uint8_t make_constant(Value value);
  static void number();
  static void grouping();
  static void unary();
  static void binary();

public:
  static void parse_precedence(Precedence precedence);
  static ParseRule *get_rule(TokenType type);
};

} // namespace Luna::Compiler

#endif
