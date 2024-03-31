#include "lexer.h"
#include "parser.h"
#include "validation.h"
#include "translation.h"

int main(void)
{
  std::string s = "block { asm! (\"mov {} 5\", out(reg) x)}";
  std::string s2 = "asm! (\"mov {} 5\", out(reg) x)";
  std::string s3 = "\"mov {} 4\",\"test instr\",inout(reg_abcd) x, in (\"eax\") y, out(reg_abcd) k, option(pure,nomem),clobber_abi(\"C\",\"test\")";
  std::vector<char> v(s3.begin(), s3.end());
  Lexer lexer(v);
  lexer.lex();
  std::cout << s3 << std::endl;
  Parser parser(lexer);
  auto ast = parser.parse();
  std::cout << "Dumping AST" << std::endl;
  parser.dump();
  std::cout << "Validating AST" << std::endl;
  validate_ast(ast);
  std::cout << "Translating AST" << std::endl;
  print_translated_node(ast).dump();
  return 0;
}
