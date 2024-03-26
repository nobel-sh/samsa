#ifndef PARSER_H_
#define PARSER_H_

#include "ast.h"
#include "token.h"
#include "lexer.h"


class Parser{
public:
  Parser(Lexer l) : tokens(l.tokens) {}

  void parse();
  void parse_instruction();
  void parse_operand();
  void parse_options();
  void parse_ident();
  AsmRegOrRegClass parse_register();
  void parse_clobber_abi();
  void consume_token();
  Token peek_token();
  bool is_eof();

  private:
  std::vector<Token> tokens;
  std::vector<Token>::iterator it = tokens.begin();

  unsigned int pos = 0;
};


#endif // PARSER_H_
