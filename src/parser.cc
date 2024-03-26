#include "parser.h"
#include "ast.h"
#include "token.h"
#include <algorithm>
#include <iostream>
#include <vector>

void Parser::consume_token(){
  if (it != tokens.end()){
    it++;
    pos++;
  }
}

Token Parser::peek_token(){
  if (it != tokens.end()){
    return *it;
  }
  return Token(0, TokenKind::ENDOFFILE, "EOF");
}

bool Parser::is_eof(){
  return it == tokens.end();
}

void Parser::parse(){
  while (!is_eof()){
    if (peek_token().kind == TokenKind::IDENT){
      parse_ident();
    }else if (peek_token().kind == TokenKind::INSTR){
      std::cout << "parsing instructions" << std::endl;
      parse_instruction();
    }
    else if (peek_token().kind == TokenKind::COMMA){
      consume_token();
      continue;
    }else return;
  }
}

void Parser::parse_instruction(){
  std::vector<std::string> instrs;
  instrs.push_back(peek_token().str);
  consume_token();
  while(peek_token().kind == TokenKind::COMMA){
    consume_token();
    if(peek_token().kind != TokenKind::INSTR){
      break;
    }
    instrs.push_back(peek_token().str);
    consume_token();
  }
  for (auto instr:instrs){
    std::cout<<"Found instruction: "<< instr << std::endl;
  }
}
void Parser::parse_ident(){
    std::cout << "Parsing on [Token]: " << peek_token().str << std::endl;
    auto is_valid_dir_spec =
      std::find(allowed_dir_spec.begin(),
                allowed_dir_spec.end(),
                peek_token().str)
                != allowed_dir_spec.end();

    if (is_valid_dir_spec){
        parse_operand();
    }else if (peek_token().str == "option"){
      consume_token();
      parse_options();
    }else if(peek_token().str == "clobber_abi"){
      consume_token();
      parse_clobber_abi();
    }else{
      std::cerr<< "Unknown dir spec found" << std::endl;
      exit(1);
    }
    return;
}

void Parser::parse_operand(){
  auto type = peek_token().str;
  consume_token();
  auto reg = parse_register();
  auto expr = peek_token().str;
  consume_token();

  std::cout << type << " " << expr << std::endl;
}


void Parser::parse_options(){
  std::vector <std::string> options;
  assert(peek_token().kind == TokenKind::LPAREN);
  consume_token();
  assert(peek_token().kind == TokenKind::IDENT);
  auto lit = peek_token().str;
  consume_token();
  options.push_back(lit);
  while(peek_token().kind == TokenKind::COMMA){
    consume_token();
    auto lit = peek_token().str;
    consume_token();
    options.push_back(lit);
  }
  assert(peek_token().kind == TokenKind::RPAREN);
  consume_token();
  for (auto option:options){
    auto it = options_map.find(option);
    if (it == options_map.end()){
      std::cerr << "Illegal Option used" << std::endl;
      exit(-1);
    }
    std::cout << "Found option: " << it->first << " [Enum]: " << it->second <<std::endl;
  }
}

AsmRegOrRegClass Parser::parse_register(){
  assert(peek_token().kind == TokenKind::LPAREN);
  consume_token();
  RegOrClass type;
  if(peek_token().kind == TokenKind::INSTR){
    type = RegOrClass::REGISTER;
  }else{
    type = RegOrClass::REGISTER_CLASS;
  }
  auto reg = peek_token().str;
  consume_token();
  assert(peek_token().kind == TokenKind::RPAREN);
  consume_token();
  return AsmRegOrRegClass(type,reg);
}

void Parser::parse_clobber_abi(){
  std::vector <std::string> abis;
  assert(peek_token().kind == TokenKind::LPAREN);
  consume_token();
  assert(peek_token().kind == TokenKind::INSTR);
  auto lit = peek_token().str;
  consume_token();
  abis.push_back(lit);
  while(peek_token().kind == TokenKind::COMMA){
    consume_token();
    assert(peek_token().kind == TokenKind::INSTR);
    auto lit = peek_token().str;
    consume_token();
    abis.push_back(lit);
  }
  assert(peek_token().kind == TokenKind::RPAREN);
  consume_token();
  for(auto abi:abis){
    std::cout << "Found ABI: " << abi <<std::endl;
  }
}
