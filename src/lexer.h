#ifndef LEXER_H
#define LEXER_H

#include "token.h"
#include <cassert>
#include <ctype.h>
#include <iostream>
#include <memory>
#include <vector>

class Lexer {
public:
  Lexer(std::vector<char> source) : source(source), pos(0) {}

  void dump();
  void lex();
  std::vector<Token> tokens;

  private:
  std::vector<char> source;
  int pos;

  bool is_eof();

  void handle_raw_instruction();
  void handle_ident();


  unsigned char peek_token();
  unsigned char peek_token(int nth);
  void consume_token();
  void consume_token(int n);

};
#endif
