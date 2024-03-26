#include "ast.h"

std::vector<std::string> allowed_dir_spec = {"in", "out", "inout", "lateout", "inlateout"};


OperandType operand_type( std::string dir_spec){
    if (dir_spec == "in")
        return OperandType::IN;
    else if (dir_spec == "out")
        return OperandType::OUT;
    else if (dir_spec == "inout")
        return OperandType::INOUT;
    else if (dir_spec == "lateout")
        return OperandType::LATEOUT;
    else if (dir_spec == "inlateout")
        return OperandType::INLATEOUT;
    else{
        std::cerr << "Unknown dir spec found" << std::endl;
        exit(1);
    }
}

void Operand::dump(){
      std::cout <<"in ("<< reg_name << ") " << expr << std::endl;
}
