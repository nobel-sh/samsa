#include "lexer.h"
#include "parser.h"
#include "validation.h"
#include "translation.h"
#include <fstream>

int main(void)
{
  std::string filename = "test";
  std::ifstream file(filename);
  std::string asm_str((std::istreambuf_iterator<char>(file)),
                 std::istreambuf_iterator<char>());
                 
  std::vector<char> v(asm_str.begin(), asm_str.end());
  Lexer lexer(v);
  lexer.lex();
  // lexer.dump();
  Parser parser(lexer);
  auto ast = parser.parse();
  // std::cout << "Dumping AST" << std::endl;
  // parser.dump();
  // std::cout << "Validating AST" << std::endl;
  validate_ast(ast);
  // std::cout << "Translating AST to extended GCC inline assembly node" << std::endl;
  std::cout << "GCC Inline Assembly Node:" << std::endl;
  print_translated_node(ast).dump();
  return 0;
}
