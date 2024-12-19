#include <cstring>

#include "scanner.h"

namespace Luna::Compiler::Lexer {

void Scanner::init_scanner(const char *source) {
  this->start = source;
  this->current = source;
  this->line = 1;
}

bool Scanner::is_at_end() { return *this->current == '\0'; }

bool Scanner::match(char expected) {
  if (this->is_at_end()) {
    return false;
  }

  if (*this->current != expected) {
    return false;
  }

  this->current++;
  return true;
}

char Scanner::advance() {
  this->current++;
  return this->current[-1];
}

void Scanner::make_token(TokenType type) {
  this->token = new Token(type, this->start, (int)(this->current - this->start),
                          this->line);

  return;
}

char Scanner::peek() { return *this->current; }

void Scanner::skip_white_space() {
  for (;;) {
    char character = this->peek();

    switch (character) {
    case ' ':
    case '\r':
    case 't':
      this->advance();
      break;
    case '\n': {
      this->line++;
      this->advance();

      break;
    }
    default:
      return;
    }
  }
}

void Scanner::error_token(const char *message) {
  this->token->type = TOKEN_ERROR;
  this->token->start = message;
  this->token->length = (int)strlen(message);
}

char Scanner::peek_next() {
  if (this->is_at_end()) {
    return '\0';
  }

  return this->current[1];
}

bool Scanner::is_digit(char c) { return c >= '0' && c <= '9'; }

void Scanner::number() {
  while (this->is_digit(this->peek())) {
    this->advance();
  }

  if (this->peek() == '.' && this->is_digit(this->peek_next())) {
    this->advance();

    while (this->is_digit(this->peek())) {
      this->advance();
    }
  }

  // change and add more types like float, unknow, double, etc
  this->make_token(TOKEN_INT);
}

bool Scanner::is_alpha(char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

TokenType Scanner::check_keyword(int start, int length, const char *rest,
                                 TokenType type) {
  if (this->current - this->start == start + length &&
      memcmp(this->start + start, rest, length) == 0) {
    return type;
  }

  return TOKEN_IDENTIFIER;
};

TokenType Scanner::identifier_type() {
  switch (this->start[0]) {
  case 'a':
    return this->check_keyword(1, 2, "nd", TOKEN_AND);
  case 'c':
    return this->check_keyword(1, 4, "lass", TOKEN_CLASS);
  case 'e':
    return this->check_keyword(1, 3, "lse", TOKEN_ELSE);
  case 'f': {
    if (this->current - this->start > 1) {
      switch (this->start[1]) {
      case 'a':
        return this->check_keyword(2, 3, "alse", TOKEN_FALSE);
      case 'o':
        return this->check_keyword(2, 1, "r", TOKEN_FOR);
      case 'n':
        return TOKEN_FN;
      }
    }
    break;
  }
  case 'i':
    return this->check_keyword(1, 1, "f", TOKEN_IF);
  case 'n':
    return this->check_keyword(1, 3, "ull", TOKEN_NULL);
  case 'o':
    return this->check_keyword(1, 1, "r", TOKEN_OR);
  case 'p':
    return this->check_keyword(1, 4, "rint", TOKEN_PRINT);
  case 'r':
    return this->check_keyword(1, 5, "eturn", TOKEN_RETURN);
  case 's':
    return this->check_keyword(1, 4, "uper", TOKEN_SUPER);
  case 'v':
    return this->check_keyword(1, 2, "ar", TOKEN_VAR);
  case 'w':
    return this->check_keyword(1, 4, "hile", TOKEN_WHILE);
  }
  return TOKEN_IDENTIFIER;
}

void Scanner::identifier() {
  while (this->is_alpha(this->peek()) || this->is_digit(this->peek())) {
    this->advance();
  }

  this->make_token(this->identifier_type());
}

void Scanner::scan_token() {
  this->skip_white_space();
  this->start = this->current;

  if (is_at_end()) {
    this->make_token(TOKEN_EOF);
    return;
  }

  char character = this->advance();

  if (this->is_alpha(character)) {
    this->identifier();
    return;
  }

  if (this->is_digit(character)) {
    this->number();
    return;
  }

  switch (character) {
  case '(': {
    return this->make_token(TOKEN_LEFT_PAREN);
  }
  case ')': {
    return this->make_token(TOKEN_RIGHT_PAREN);
  }
  case '{': {
    return this->make_token(TOKEN_LEFT_BRACE);
  }
  case '}': {
    return this->make_token(TOKEN_RIGHT_BRACE);
  }
  case ';': {
    return this->make_token(TOKEN_SEMICOLON);
  }
  case ',': {
    return this->make_token(TOKEN_COMMA);
  }
  case '.': {
    return this->make_token(TOKEN_DOT);
  }
  case '-': {
    return this->make_token(TOKEN_MINUS);
  }
  case '+': {
    return this->make_token(TOKEN_PLUS);
  }
  case '/': {
    if (this->peek_next() == '/') {
      while (this->peek() != '\n' && !this->is_at_end()) {
        this->advance();
      }
    } else {
      return;
    }
    // this->make_token(TOKEN_SLASH);
    break;
  }
  case '*': {
    return this->make_token(TOKEN_START);
  }
  case '!': {
    return this->make_token(this->match('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
  }
  case '=':
    return this->make_token(this->match('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
  case '<': {
    return this->make_token(this->match('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
  }
  case '>': {
    return this->make_token(this->match('=') ? TOKEN_GREATER_EQUAL
                                             : TOKEN_GREATER);
  }
  case '"': {
    return this->string();
  }
  }

  return this->error_token("Unexpected character");
}

void Scanner::string() {
  while (this->peek() != '"' && !this->is_at_end()) {
    if (this->peek() == '{') {
      this->advance();

      while (this->peek() != '}' && !this->is_at_end()) {
        this->advance();
      }

      if (this->is_at_end()) {
        this->error_token("Unterminated string.");
        return;
      }
      this->advance();
    } else {
      if (this->peek() == '\n') {
        this->line++;
      }

      this->advance();
    }
  }

  if (this->is_at_end()) {
    this->error_token("Unterminated string.");
    return;
  }

  // closing quotes
  this->advance();
  this->make_token(TOKEN_STRING);
}

} // namespace Luna::Compiler::Lexer
