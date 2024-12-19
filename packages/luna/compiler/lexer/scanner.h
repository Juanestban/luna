#ifndef LUNA_COMPILER_SCANNER_H
#define LUNA_COMPILER_SCANNER_H

#include "../common.h"

namespace Luna::Compiler::Lexer {

enum TokenType {
  // Single character tokens
  TOKEN_LEFT_PAREN,
  TOKEN_RIGHT_PAREN,
  TOKEN_LEFT_BRACE,
  TOKEN_RIGHT_BRACE,
  TOKEN_COMMA,
  TOKEN_DOT,
  TOKEN_MINUS,
  TOKEN_PLUS,
  TOKEN_SEMICOLON,
  TOKEN_SLASH,
  TOKEN_STAR,

  // oNE OR Two character tokens
  TOKEN_BANG,
  TOKEN_BANG_EQUAL,
  TOKEN_EQUAL,
  TOKEN_EQUAL_EQUAL,
  TOKEN_GREATER,
  TOKEN_GREATER_EQUAL,
  TOKEN_LESS,
  TOKEN_LESS_EQUAL,

  // Literals
  TOKEN_IDENTIFIER,
  TOKEN_STRING,
  TOKEN_INT,
  TOKEN_FLOAT,
  TOKEN_BOOL,
  TOKEN_CONST,

  // Keywords
  TOKEN_AND,
  TOKEN_CLASS,
  TOKEN_FALSE,
  TOKEN_TRUE,
  TOKEN_FOR,
  TOKEN_FN,
  TOKEN_IF,
  TOKEN_ELIF,
  TOKEN_ELSE,
  TOKEN_OR,
  TOKEN_NULL,
  TOKEN_PRINT,
  TOKEN_RETURN,
  TOKEN_SUPER,
  TOKEN_THIS,
  TOKEN_EXTENDS,
  TOKEN_VAR,
  TOKEN_WHILE,

  TOKEN_ERROR,
  TOKEN_EOF
};

class Token {
public:
  TokenType type;
  const char *start;
  int length;
  int line;

public:
  Token(TokenType _type, const char *_start, int _length, int _line)
      : type(_type), start(_start), length(_length), line(_line) {}
};

class Scanner {
public:
  const char *start;
  const char *current;
  Token *token;
  int line;

public:
  char advance();
  char peek();
  char peek_next();

public:
  bool match(char expected);
  bool is_at_end();
  bool is_digit(char c);
  bool is_alpha(char c);

public:
  TokenType identifier_type();
  TokenType check_keyword(int start, int length, const char *rest,
                          TokenType type);

public:
  void init_scanner(const char *source);
  void scan_token();
  void make_token(TokenType type);
  void error_token(const char *message);
  void skip_white_space();
  void string();
  void number();
  void identifier();
};

} // namespace Luna::Compiler::Lexer

#endif
