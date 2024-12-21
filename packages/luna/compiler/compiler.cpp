#include "compiler.h"
#include "chunk.h"
#include "lexer/scanner.h"
#include "value.h"

namespace Luna::Compiler {

using namespace Lexer;

Scanner *LunaCompiler::scanner;
Parser *LunaCompiler::parser;
Chunk *LunaCompiler::chunk;

ParseRule LunaCompiler::rules[] = {
    {grouping, NULL, PREC_NONE},     // TOKEN_LEFT_PAREN
    {NULL, NULL, PREC_NONE},         // TOKEN_RIGHT_PAREN
    {NULL, NULL, PREC_NONE},         // TOKEN_LEFT_BRACE
    {NULL, NULL, PREC_NONE},         // TOKEN_RIGHT_BRACE
    {NULL, NULL, PREC_NONE},         // TOKEN_COMMA
    {NULL, NULL, PREC_NONE},         // TOKEN_DOT
    {unary, binary, PREC_TERM},      // TOKEN_MINUS
    {NULL, binary, PREC_TERM},       // TOKEN_PLUS
    {NULL, NULL, PREC_NONE},         // TOKEN_SEMICOLON
    {NULL, binary, PREC_FACTOR},     // TOKEN_SLASH
    {NULL, binary, PREC_FACTOR},     // TOKEN_STAR
    {unary, NULL, PREC_NONE},        // TOKEN_BANG
    {NULL, binary, PREC_EQUALITY},   // TOKEN_BANG_EQUAL
    {NULL, NULL, PREC_NONE},         // TOKEN_EQUAL
    {NULL, binary, PREC_EQUALITY},   // TOKEN_EQUAL_EQUAL
    {NULL, binary, PREC_COMPARISON}, // TOKEN_GREATER
    {NULL, binary, PREC_COMPARISON}, // TOKEN_GREATER_EQUAL
    {NULL, binary, PREC_COMPARISON}, // TOKEN_LESS
    {NULL, binary, PREC_COMPARISON}, // TOKEN_LESS_EQUAL
    {NULL, NULL, PREC_NONE},         // TOKEN_IDENTIFIER
    {NULL, NULL, PREC_NONE},         // TOKEN_STRING
    {number, NULL, PREC_NONE},       // TOKEN_INT
    {number, NULL, PREC_NONE},       // TOKEN_FLOAT
    {NULL, NULL, PREC_NONE},         // TOKEN_BOOL
    {NULL, NULL, PREC_NONE},         // TOKEN_OBJ
    {NULL, NULL, PREC_NONE},         // TOKEN_CONSTANT
    {NULL, NULL, PREC_NONE},         // TOKEN_AND
    {NULL, NULL, PREC_NONE},         // TOKEN_CLASS
    {literal, NULL, PREC_NONE},      // TOKEN_FALSE
    {literal, NULL, PREC_NONE},      // TOKEN_TRUE
    {NULL, NULL, PREC_NONE},         // TOKEN_FOR
    {NULL, NULL, PREC_NONE},         // TOKEN_FN
    {NULL, NULL, PREC_NONE},         // TOKEN_IF
    {NULL, NULL, PREC_NONE},         // TOKEN_ELIF
    {NULL, NULL, PREC_NONE},         // TOKEN_ELSE
    {NULL, NULL, PREC_NONE},         // TOKEN_OR
    {literal, NULL, PREC_NONE},      // TOKEN_NULL
    {NULL, NULL, PREC_NONE},         // TOKEN_PRINT
    {NULL, NULL, PREC_NONE},         // TOKEN_RETURN
    {NULL, NULL, PREC_NONE},         // TOKEN_SUPER
    {NULL, NULL, PREC_NONE},         // TOKEN_THIS
    {NULL, NULL, PREC_NONE},         // TOKEN_EXTENDS
    {NULL, NULL, PREC_NONE},         // TOKEN_VAR
    {NULL, NULL, PREC_NONE},         // TOKEN_WHILE
    {NULL, NULL, PREC_NONE},         // TOKEN_ERROR
    {NULL, NULL, PREC_NONE},         // TOKEN_EOF
};

void LunaCompiler::literal() {
  switch (LunaCompiler::parser->previous->type) {
  case TOKEN_FALSE:
    LunaCompiler::emit_byte(OP_FALSE);
    break;
  case TOKEN_NULL:
    LunaCompiler::emit_byte(OP_NULL);
    break;
  case TOKEN_TRUE:
    LunaCompiler::emit_byte(OP_TRUE);
    break;
  default:
    return; // Unreachable
  }
}

void LunaCompiler::parse_precedence(Precedence precedence) {
  LunaCompiler::advance();

  ParseFn prefix_rule = LunaCompiler::get_rule(parser->previous->type)->prefix;

  if (prefix_rule == nullptr) {
    parser->error_at_current("Expected expression.");
    return;
  }

  prefix_rule();

  while (precedence <=
         LunaCompiler::get_rule(parser->current->type)->precedence) {
    LunaCompiler::advance();

    ParseFn infix_rule = LunaCompiler::get_rule(parser->previous->type)->infix;
    infix_rule();
  }
};

ParseRule *LunaCompiler::get_rule(TokenType type) {
  return &LunaCompiler::rules[type];
}

void LunaCompiler::advance() {
  parser->previous = parser->current;

  for (;;) {
    scanner->scan_token();

    if (parser->current->type != TOKEN_ERROR) {
      break;
    }

    parser->error_at_current(parser->current->start);
  }
}

void LunaCompiler::emit_byte(uint8_t byte) {
  chunk->write_me(byte, parser->previous->line);
}

void LunaCompiler::emit_bytes(uint8_t byte1, uint8_t byte2) {
  LunaCompiler::emit_byte(byte1);
  LunaCompiler::emit_byte(byte2);
}

void LunaCompiler::emit_return() { LunaCompiler::emit_byte(OP_RETURN); }

void LunaCompiler::end_compiler() {
  LunaCompiler::emit_return();

#ifdef DEBUG_PRINT_CODE
  if (!parser->had_error) {
    disassemble_chunk(chunk, "code");
  }
#endif
}

void LunaCompiler::expression() {
  LunaCompiler::parse_precedence(PREC_ASSIGNMENT);
};

void LunaCompiler::number() {
  double value = strtod(parser->previous->start, nullptr);

  LunaCompiler::emit_constant(NUMBER_VAL(value));
};

void LunaCompiler::emit_constant(Value value) {
  LunaCompiler::emit_bytes(OP_CONSTANT, LunaCompiler::make_constant(value));
};

void LunaCompiler::grouping() {
  LunaCompiler::expression();

  parser->consume(TOKEN_RIGHT_PAREN, "Expect ')' after expression.");
};

void LunaCompiler::unary() {
  TokenType operator_type = parser->previous->type;

  LunaCompiler::parse_precedence(PREC_UNARY);

  switch (operator_type) {
  case TOKEN_BANG:
    LunaCompiler::emit_byte(OP_NOT);
    break;
  case TOKEN_MINUS:
    LunaCompiler::emit_byte(OP_NEGATE);
    break;
  default:
    return; // Unreachable
  }
};

void LunaCompiler::binary() {
  TokenType operator_type = parser->previous->type;
  ParseRule *rule = LunaCompiler::get_rule(operator_type);
  LunaCompiler::parse_precedence((Precedence)(rule->precedence + 1));

  switch (operator_type) {
  case TOKEN_BANG_EQUAL:
    LunaCompiler::emit_byte(OP_EQUAL);
    break;
  case TOKEN_EQUAL_EQUAL:
    LunaCompiler::emit_byte(OP_EQUAL);
    break;
  case TOKEN_GREATER:
    LunaCompiler::emit_byte(OP_GREATER);
    break;
  case TOKEN_GREATER_EQUAL:
    LunaCompiler::emit_byte(OP_LESS);
    break;
  case TOKEN_LESS:
    LunaCompiler::emit_byte(OP_LESS);
    break;
  case TOKEN_LESS_EQUAL:
    LunaCompiler::emit_byte(OP_GREATER);
    break;
  case TOKEN_PLUS:
    LunaCompiler::emit_byte(OP_ADD);
    break;
  case TOKEN_MINUS:
    LunaCompiler::emit_byte(OP_SUBTRACT);
    break;
  case TOKEN_STAR:
    LunaCompiler::emit_byte(OP_MULTIPLY);
    break;
  case TOKEN_SLASH:
    LunaCompiler::emit_byte(OP_DIVIDE);
    break;
  default:
    return; // Unreachable
  }
}

uint8_t LunaCompiler::make_constant(Value value) {
  int constant = chunk->add_constant(value);

  if (constant > UINT8_MAX) {
    parser->error_at_current("Too many constants in one chunk.");

    return 0;
  }

  return (uint8_t)constant;
};

bool LunaCompiler::compile(const char *source, Chunk *_chunk) {
  scanner = new Scanner();
  parser = new Parser(scanner);

  scanner->init_scanner(source);
  chunk = _chunk;

  parser->had_error = false;
  parser->panic_mode = false;

  parser->advance();

  LunaCompiler::expression();

  parser->consume(TOKEN_EOF, "Expect end of expression.");
  LunaCompiler::end_compiler();

  return !parser->had_error;
};

} // namespace Luna::Compiler
