#include "lexer.h"

void Lexer::lex()
{
  while (!is_eof())
  {
    // std::cout << "current pos: " << pos << std::endl;
    auto curr_char = peek_token();
    // std::cout << "current char: " << curr_char << std::endl;
    switch (curr_char)
    {
    case '(':
      tokens.push_back(
          Token(pos, TokenKind::LPAREN, "("));
      break;
    case ')':
      tokens.push_back(
          Token(pos, TokenKind::RPAREN, ")"));
      break;
    case ' ':
      while (peek_token() == ' ')
        consume_token();
      continue;
    case '{':
      tokens.push_back(
          Token(pos, TokenKind::LBRACE, "{"));
      break;
    case '}':
      tokens.push_back(
          Token(pos, TokenKind::RBRACE, "}"));
      break;
    case ',':
      tokens.push_back(
          Token(pos, TokenKind::COMMA, ","));
      break;
    case '!':
      tokens.push_back(
          Token(pos, TokenKind::BANG, "!"));
      break;
    case '=':
      tokens.push_back(
          Token(pos, TokenKind::EQUALS, "="));
      break;
    case '_':
      tokens.push_back(
          Token(pos, TokenKind::UNDERSCORE, "_"));
      break;
    case '"':
      handle_raw_instruction();
      continue;
    default:
      if (isalpha(curr_char))
      {
        handle_ident();
        continue;
      }
      else
      {
        // there are better ways to handle this. just suppressing error for now
        // std::cerr << "Unknown token found at: " << pos << std::endl;
        // std::cerr << "Token is: " << curr_char << std::endl;
      }
      break;
    }
    consume_token();
  }
  tokens.push_back(
      Token(pos, TokenKind::ENDOFFILE, "EOF"));
}

void Lexer::handle_raw_instruction()
{
  consume_token();
  char curr_char = peek_token();
  auto start_pos = pos;
  while (curr_char != '"')
  {
    curr_char = peek_token();
    if (is_eof())
      std::cerr << "Non terminated String at: " << pos << std::endl;
    consume_token();
  }
  std::string instruction(source.begin() + start_pos, source.begin() + pos - 1);
  tokens.push_back(
      Token(start_pos, TokenKind::INSTR, std::move(instruction)));
}

void Lexer::handle_ident()
{
  auto start_pos = pos;
  while (isalnum(peek_token()) || peek_token() == '_')
  {
    consume_token();
  }
  std::string str(source.begin() + start_pos, source.begin() + pos);
  tokens.push_back(
      Token(start_pos, TokenKind::IDENT, std::move(str)));
}

void Lexer::dump()
{
  for (auto tok : tokens)
  {
    tok.dump();
  }
}

unsigned char
Lexer::peek_token() { return peek_token(0); }

unsigned char
Lexer::peek_token(int nth)
{
  if (pos + nth >= source.size())
    return EOF;
  return source[pos + nth];
}

void Lexer::consume_token() { consume_token(1); }

void Lexer::consume_token(int n)
{
  if (pos + n >= source.size())
    pos = source.size();
  else
    pos += n;
}

bool Lexer::is_eof() { return pos >= source.size(); }
