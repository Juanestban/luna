#ifndef LUNA_COMPILER_LEXER_PARSER_H
#define LUNA_COMPILER_LEXER_PARSER_H

#include "../chunk.h"
#include "../common.h"
#include "scanner.h"

namespace Luna::Compiler::Lexer {

class Parser {
public:
  Token *current;
  Token *previous;
  Scanner *scanner;
  bool had_error;
  bool panic_mode;

public:
  Parser() {}

public:
  void advance();
  void expression();
  void consume(TokenType type, const char *message);
  void error_at_current(const char *message);
  void error_at(const char *message);
};

} // namespace Luna::Compiler::Lexer

#endif
