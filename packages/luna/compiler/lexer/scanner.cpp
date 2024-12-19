#include "scanner.h"
#include <cstring>

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

TokenType Scanner::identifier_type() { return TOKEN_IDENTIFIER; }

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
    this->make_token(TOKEN_LEFT_PAREN);
    break;
  }
  case ')': {
    this->make_token(TOKEN_RIGHT_PAREN);
    break;
  }
  case '{': {
    this->make_token(TOKEN_LEFT_BRACE);
    break;
  }
  case '}': {
    this->make_token(TOKEN_RIGHT_BRACE);
    break;
  }
  case ';': {
    this->make_token(TOKEN_SEMICOLON);
    break;
  }
  case ',': {
    this->make_token(TOKEN_COMMA);
    break;
  }
  case '.': {
    this->make_token(TOKEN_DOT);
    break;
  }
  case '-': {
    this->make_token(TOKEN_MINUS);
    break;
  }
  case '+': {
    this->make_token(TOKEN_PLUS);
    break;
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
    this->make_token(TOKEN_START);
    break;
  }
  case '!': {
    this->make_token(this->match('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
    break;
  }
  case '=':
    this->make_token(this->match('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
    break;
  case '<': {
    this->make_token(this->match('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
    break;
  }
  case '>': {
    this->make_token(this->match('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);
    break;
  }
  case '"': {
    this->string();
    break;
  }
  }

  this->error_token("Unexpected character");
}

void Scanner::string() {
  while (this->peek() != '"' && !this->is_at_end()) {
    if (this->peek() == '\n') {
      this->line++;
    }

    this->advance();
  }

  if (this->is_at_end()) {
    this->error_token("Unterminated string.");
    return;
  }

  this->advance();
  this->make_token(TOKEN_STRING);
}

} // namespace Luna::Compiler::Lexer
