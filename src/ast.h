#ifndef AST_H_
#define AST_H_

#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>

extern std::vector<std::string> allowed_dir_spec;
enum OperandType{
        IN,
        OUT,
        INOUT,
        LATEOUT,
        INLATEOUT,
};
OperandType operand_type( std::string dir_spec);

enum RegOrClass{
     REGISTER,
     REGISTER_CLASS,
};

class AsmRegOrRegClass{
public:
    RegOrClass type;
    //NOTE: Only explicit registers for now
    std::string name;
    AsmRegOrRegClass(RegOrClass t, std::string &n)
      : type(t), name(n)
    {}
};

class Operand{
public:
    OperandType type;
    std::string reg_name;
    std::string expr; // expression it holds
    Operand(std::string &reg_name, std::string &expr, OperandType type)
      : reg_name(reg_name), expr(expr), type(type){}
    void dump();
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

static std::unordered_map<std::string,InlineAsmOption> options_map = {
  {"pure", InlineAsmOption::PURE},
  {"nomem", InlineAsmOption::NOMEM},
  {"readonly", InlineAsmOption::READONLY},
  {"preserved_flags", InlineAsmOption::PRESERVES_FLAGS},
  {"noreturn", InlineAsmOption::NORETURN},
  {"nostack", InlineAsmOption::NOSTACK},
  {"att_syntax", InlineAsmOption::ATT_SYNTAX},
  {"raw", InlineAsmOption::RAW},
  {"may_unwind", InlineAsmOption::MAY_UNWIND},
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
