#ifndef AST_H_
#define AST_H_

#include <string>
#include <vector>
#include <iostream>


extern std::vector<std::string> allowed_dir_spec;
enum OperandType{
        IN,
        OUT,
        INOUT,
        LATEOUT,
        INLATEOUT,
};
OperandType operand_type( std::string dir_spec);

class Operand{
    OperandType type;
    std::string reg_name;
    std::string expr; // expression it holds
    Operand(std::string &&reg_name, std::string &&expr, OperandType type)
      : reg_name(reg_name), expr(expr), type(type){}
};

// raw strings only for now
class AsmTemplate{
public:
    std::string asm_str;
    AsmTemplate(std::string &&asm_str)
      : asm_str(asm_str) {}
};

enum InlineAsmOption{
  PURE,
  NOMEM,
  READONLY,
  PRESERVES_FLAGS,
  NORETURN,
  NOSTACK,
  ATT_SYNTAX,
  RAW,
  MAY_UNWIND,
  UNKNOWN,
};

class Clobber{
    std::string clobber;
    Clobber(std::string &&clobber)
      : clobber (clobber) {}
};

class InlineAsm{
public:
    std::vector<AsmTemplate> templates;
    std::vector<InlineAsmOption> options;
    std::vector<Operand> operands;
    Clobber clobbers;

    InlineAsm(std::vector<AsmTemplate> t, std::vector<InlineAsmOption> o,
              std::vector<Operand> op, Clobber c)
      : templates(t), options(o), operands(op), clobbers(c){}
};



#endif // AST_H_
