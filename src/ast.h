#ifndef AST_H_
#define AST_H_

#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>

static std::vector<std::string> allowed_dir_spec =
    {"in", "out", "inout", "lateout", "inlateout"};

enum OperandType
{
  IN,
  OUT,
  INOUT,
  LATEOUT,
  INLATEOUT,
};

enum RegOrClass
{
  REGISTER,
  REGISTER_CLASS,
};

// Only allows a single register type
// doesnot allow _ => _ type yet
class Operand
{
public:
  OperandType type;
  RegOrClass reg_type;
  std::string reg_name;
  std::string expr; // string of expression it holds for now
  Operand(OperandType type, RegOrClass reg_type,
          std::string &reg_name, std::string &expr)
      : reg_name(reg_name), reg_type(reg_type),
        expr(expr), type(type) {}
  void dump();
};

// raw strings only for now
class AsmTemplate
{
public:
  std::string asm_str;
  AsmTemplate(std::string &asm_str)
      : asm_str(asm_str) {}
  void dump();
};

enum AsmOption
{
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

static std::unordered_map<std::string, AsmOption> options_map = {
    {"pure", AsmOption::PURE},
    {"nomem", AsmOption::NOMEM},
    {"readonly", AsmOption::READONLY},
    {"preserved_flags", AsmOption::PRESERVES_FLAGS},
    {"noreturn", AsmOption::NORETURN},
    {"nostack", AsmOption::NOSTACK},
    {"att_syntax", AsmOption::ATT_SYNTAX},
    {"raw", AsmOption::RAW},
    {"may_unwind", AsmOption::MAY_UNWIND},
};

static std::unordered_map<AsmOption, std::string> options_to_string = {
    {AsmOption::PURE, "pure"},
    {AsmOption::NOMEM, "nomem"},
    {AsmOption::READONLY, "readonly"},
    {AsmOption::PRESERVES_FLAGS, "preserved_flags"},
    {AsmOption::NORETURN, "noreturn"},
    {AsmOption::NOSTACK, "nostack"},
    {AsmOption::ATT_SYNTAX, "att_syntax"},
    {AsmOption::RAW, "raw"},
    {AsmOption::MAY_UNWIND, "may_unwind"},
};

class Clobber
{
  std::string clobber;

public:
  Clobber(std::string &clobber)
      : clobber(clobber) {}
  void dump();
};

class InlineAsm
{
public:
  std::vector<AsmTemplate> templates;
  std::vector<AsmOption> options;
  std::vector<Operand> operands;
  std::vector<Clobber> clobbers;
  InlineAsm(std::vector<AsmTemplate> t, std::vector<AsmOption> o,
            std::vector<Operand> op, std::vector<Clobber> c)
      : templates(t), options(o), operands(op), clobbers(c) {}
  InlineAsm() {}
  void dump();
};

#endif // AST_H_
