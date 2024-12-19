#include <cstdio>

#include "compiler.h"
#include "lexer/scanner.h"

namespace Luna::Compiler {

using namespace Lexer;

void compile(const char *source) {
  Scanner *scanner = new Scanner();

  scanner->init_scanner(source);

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
