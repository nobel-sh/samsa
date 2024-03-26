#include "lexer.h"
#include "parser.h"

int main(void) {
  std::string s = "block { asm! (\"mov {} 5\", out(reg) x)}";
  std::string s2 = "asm! (\"mov {} 5\", out(reg) x)";
  std::string s3 = "\"mov {} 4\",\"test instr\",out(reg) x, option(pure,nomem),clobber_abi(\"C\",\"test\")";
  std::vector<char> v(s3.begin(), s3.end());
  Lexer lexer(v);
  lexer.lex();
  std::cout<< s3 << std::endl;
  Parser parser(lexer);
  parser.parse();
  return 0;
}
