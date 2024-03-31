#ifndef TOKEN_H_
#define TOKEN_H_

#include <iostream>
#include <memory>
#include <map>

enum TokenKind
{
  IDENT,
  BANG,
  LPAREN,
  RPAREN,
  EQUALS,
  LBRACE,
  RBRACE,
  INSTR,
  COMMA,
  UNDERSCORE,
  ENDOFFILE,
};

class Token
{
public:
  Token(unsigned int pos, TokenKind kind, std::string &&str)
      : pos(pos), kind(kind), str(str) {}

  void dump();
  std::string to_string(TokenKind kind);

  TokenKind kind;
  std::string str;

private:
  unsigned int pos;

  std::map<TokenKind, std::string> token_string = {
      {TokenKind::IDENT, "IDENT"},
      {TokenKind::BANG, "BANG"},
      {
          TokenKind::LPAREN,
          "LPAREN",
      },
      {
          TokenKind::RPAREN,
          "RPAREN",
      },
      {
          TokenKind::EQUALS,
          "EQUALS",
      },
      {TokenKind::LBRACE, "LBRACE,"},
      {TokenKind::RBRACE, "RBRACE,"},
      {TokenKind::INSTR, "INSTR,"},
      {TokenKind::COMMA, "COMMA"},
      {TokenKind::ENDOFFILE, "ENDOFFILE"},

  };
};

#endif // TOKEN_H_
