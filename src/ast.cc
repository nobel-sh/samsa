#include "ast.h"

void Operand::dump(){
      std::cout <<"[Operands]: dir_spec type: "<< type << " ("
                << "Type:" << reg_type << " Name: " << reg_name
                << ") " << expr << std::endl;
}
void AsmTemplate::dump(){
      std::cout << "[Template]: " << asm_str << std::endl;
}

void Clobber::dump(){
      std::cout << "[Clobber]: " << clobber << std::endl;
}


void InlineAsm::dump(){
    for (auto x: operands)
      x.dump();

    for (auto x: templates)
      x.dump();

    for (auto x: clobbers)
      x.dump();

    for (auto option : options) {
      auto it = options_to_string.find(option);
      std::cout << "[Option]: ";
      std::cout << it->second << std::endl;
    }
}
