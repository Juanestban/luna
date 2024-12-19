#include <cstdio>

#include "compiler.h"

namespace Luna::Compiler {

using namespace Lexer;

bool compile(const char *source, Chunk *chunk) {
  Scanner *scanner = new Scanner();
  Parser *parser = new Parser();

  scanner->init_scanner(source);

  parser->had_error = false;
  parser->panic_mode = false;

  parser->advance();
  parser->expression();

  parser->consume(TOKEN_EOF, "Expect end of expression.");
  return !parser->had_error;

  int line = -1;

  for (;;) {
    scanner->scan_token();

    if (scanner->token->line != line) {
      printf("%4d ", scanner->token->line);

      line = scanner->token->line;
    } else {
      printf("   | ");
    }

    printf("%2d '%.*s'\n", scanner->token->type, scanner->token->length,
           scanner->token->start);

    if (scanner->token->type == TOKEN_EOF)
      break;
  }
};

} // namespace Luna::Compiler
