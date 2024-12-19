#include "parser.h"
#include <cstdio>

namespace Luna::Compiler::Lexer {

void Parser::consume(TokenType type, const char *message) {
  if (this->current->type == type) {
    this->advance();
    return;
  }

  this->error_at_current(message);
}

void Parser::error_at(const char *message) {
  if (this->panic_mode) {
    return;
  }

  this->panic_mode = true;

  fprintf(stderr, "[line %d] Error", this->current->line);

  if (this->previous->type == TOKEN_EOF) {
    fprintf(stderr, " at end");
  } else if (this->previous->type == TOKEN_ERROR) {
    // Nothing.
  } else {
    fprintf(stderr, " at '%.*s'", this->previous->length,
            this->previous->start);
  }

  fprintf(stderr, ": %s\n", message);

  this->had_error = true;
}

void Parser::error_at_current(const char *message) { this->error_at(message); }

void Parser::advance() {
  this->previous = this->current;

  for (;;) {
    this->scanner->scan_token();
    this->current = this->scanner->token;

    if (this->current->type != TOKEN_ERROR) {
      break;
    }

    this->error_at_current(this->current->start);
  }
}

} // namespace Luna::Compiler::Lexer
