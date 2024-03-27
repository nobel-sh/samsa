#ifndef AST_H_
#define AST_H_

#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>

static std::vector<std::string> allowed_dir_spec =
         {"in", "out", "inout", "lateout", "inlateout"};

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

class Operand{
public:
  OperandType type;
  RegOrClass reg_type;
  std::string reg_name;
  std::string expr; // string of expression it holds for now
  Operand( OperandType type, RegOrClass reg_type,
           std::string &reg_name, std::string &expr)
    : reg_name(reg_name), reg_type(reg_type),
      expr(expr), type(type){}
  void dump();
};

// raw strings only for now
class AsmTemplate{
public:
  std::string asm_str;
  AsmTemplate(std::string &asm_str)
    : asm_str(asm_str) {}
  void dump();
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

static std::unordered_map<std::string, InlineAsmOption> options_map = {
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

static std::unordered_map<InlineAsmOption, std::string> options_to_string = {
  {InlineAsmOption::PURE, "pure" },
  {InlineAsmOption::NOMEM, "nomem"} ,
  {InlineAsmOption::READONLY, "readonly" },
  {InlineAsmOption::PRESERVES_FLAGS, "preserved_flags"},
  {InlineAsmOption::NORETURN, "noreturn"},
  {InlineAsmOption::NOSTACK, "nostack"},
  {InlineAsmOption::ATT_SYNTAX, "att_syntax"},
  {InlineAsmOption::RAW, "raw"},
  {InlineAsmOption::MAY_UNWIND, "may_unwind"},
};


class Clobber{
  std::string clobber;
public:
  Clobber(std::string &clobber)
      : clobber (clobber) {}
  void dump();
};

class InlineAsm{
public:
  std::vector<AsmTemplate> templates;
  std::vector<InlineAsmOption> options;
  std::vector<Operand> operands;
  std::vector<Clobber> clobbers;
  InlineAsm(std::vector<AsmTemplate> t, std::vector<InlineAsmOption> o,
            std::vector<Operand> op, std::vector<Clobber> c)
    : templates(t), options(o), operands(op), clobbers(c){}
  InlineAsm(){}
  void dump();
};


#endif //AST_H_
