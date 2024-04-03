#include "translation.h"

GCCInline print_translated_node(InlineAsm &node)
{
    auto gcc_node = GCCInline();
    for (auto x : node.operands)
    {
        translate_reg(x, gcc_node);
    }
    gcc_node.templated_strs = translate_template(node);
    return gcc_node;
}

// https://github.com/rust-lang/rust/blob/master/compiler/rustc_codegen_gcc/src/asm.rs
//
// With that in mind, let's see how we translate Rust syntax to GCC
// (from now on, `CC` stands for "constraint code"):
//
// * `out(reg_class) var`   -> translated to output operand: `"=CC"(var)`
// * `inout(reg_class) var` -> translated to output operand: `"+CC"(var)`
// * `in(reg_class) var`    -> translated to input operand: `"CC"(var)`
//
// * `out(reg_class) _` -> translated to one `=r(tmp)`, where "tmp" is a
// temporary unused variable
//
// * `out("explicit register") _` -> not translated to any operands, register is
// simply added to clobbers list
//
// * `inout(reg_class) in_var => out_var` -> translated to two operands:
//                              output: `"=CC"(in_var)`
//                              input:  `"num"(out_var)` where num is the GCC
//                              index
//                                       of the corresponding output operand
//
// * `inout(reg_class) in_var => _` -> same as `inout(reg_class) in_var => tmp`,
//                                      where "tmp" is a temporary unused
//                                      variable
//
// * `out/in/inout("explicit register") var` -> translated to one or two
// operands as described above with `"r"(var)` constraint, and one register
// variable assigned to the desired register.

int arbitary_var_index = 0;
// doesnot handle the "=>" case for now
void translate_reg(Operand &reg, GCCInline &gcc_inline)
{
    if (reg.expr == "_")
    {
        if (reg.type == OperandType::OUT)
        {
            if (reg.reg_type == RegOrClass::REGISTER)
            {
                gcc_inline.clobbers.push_back("\"" + reg.reg_name + "\"");
                return;
            }
            std::string tmp = "tmp" + std::to_string(arbitary_var_index++);
            gcc_inline.output_operands.push_back("\"=r\" (" + tmp + ")");
        }
        else
        {
            std::cout << "Invalid input operand" << std::endl;
            exit(1);
        }
        return;
    }

    if (reg.type == OperandType::IN || reg.type == OperandType::OUT ||
        reg.type == OperandType::INOUT)
    {
        std::string constraint;
        auto expression = reg.expr;
        if (reg.reg_type == RegOrClass::REGISTER)
        {
            constraint = "r";
        }
        else
        {
            constraint = give_reg_from_reg_class(reg.reg_name);
        }
        if (reg.type == OperandType::IN)
        {
            auto final_str = "\"" + constraint + "\" (" + expression + ")";
            gcc_inline.input_operands.push_back(final_str);
        }
        if (reg.type == OperandType::OUT)
        {
            auto final_str = "\"=" + constraint + "\" (" + expression + ")";
            gcc_inline.output_operands.push_back(final_str);
        }
        if (reg.type == OperandType::INOUT)
        {
            auto final_str = "\"+" + constraint + "\" (" + expression + ")";
            gcc_inline.output_operands.push_back(final_str);
        }
    }
    else
    {
        std::cout << "Invalid operand type" << std::endl;
        exit(1);
    }
}

std::unordered_map<std::string, std::string> reg_to_reg_class = {
    {"reg", "r"},
    {"reg_abcd", "Q"},
    {"reg_byte", "q"},
    {"xmm_reg", "x"},
    {"ymm_reg", "x"},
    {"zmm_reg", "v"},
    {"kreg", "Yk"},
    {"kreg0", "0"},
    {"x87_reg", "0"},
    {"mmx_reg", "0"},
    {"tmm_reg", "0"},
};

std::string give_reg_from_reg_class(std::string reg)
{
    if (reg_to_reg_class.find(reg) == reg_to_reg_class.end())
    {
        std::cout << "Invalid register class" << std::endl;
        exit(1);
    }
    return reg_to_reg_class[reg];
}

std::vector<std::string> translate_template(InlineAsm &node)
{
    auto templates = node.templates;
    std::vector<std::string> final_templates;
    for (auto str : templates)
    {
        std::string local_str = "";
        for (auto c : str.asm_str)
        {
            if (c == '{')
            {
                auto start_index = str.asm_str.find('{');
                auto end_index = str.asm_str.find('}');
                if (start_index == std::string::npos || end_index == std::string::npos)
                {
                    std::cout << "Invalid template" << std::endl;
                    exit(1);
                }
                auto reg_name = str.asm_str.substr(start_index + 1, end_index).erase();
                // auto idx = node.find_reg_index(reg_name);
                // if (idx >=0 )
                // {
                //     local_str += "%" + idx;
                // } else local_str += "%" + reg_name;
                local_str += "%" + reg_name;
            }
            else if (c == '}')
            {
                continue;
            }
            else
            {
                local_str += c;
            }
        }
        // final_str += intel_to_att(local_str);
        // final_str += "\\n\\t";
        auto att = intel_to_att(local_str) + "\\n\\t";
        final_templates.push_back(att);
    }
    if (!final_templates.empty())
    {
        std::string &last_template = final_templates.back();
        if (!last_template.empty())
        {
            last_template = last_template.erase(last_template.size() - 4);
        }
    }
    return final_templates;
}

// quick conversion from intel to att syntax
std::string intel_to_att(std::string instr)
{
    std::string final_str = "";
    std::stringstream ss(instr);
    std::vector<std::string> tokens;
    std::string token;
    while (std::getline(ss, token, ' '))
    {
        if (isStringDigit(token))
        {
            tokens.push_back("$" + token);
        }
        else
            tokens.push_back(token);
    }

    if (tokens.size() == 1)
    {
        return instr;
    }
    else if (tokens.size() == 2)
    {
        final_str += tokens[0] + " ";
        final_str += tokens[1] + " ";
        return final_str;
    }
    // swap the source and destination operands
    final_str += tokens[0] + " ";
    final_str += isStringDigit(tokens[2]) ? "$" : "";
    final_str += tokens[2] + ", ";
    final_str += isStringDigit(tokens[2]) ? "$" : "";
    final_str += tokens[1];
    return final_str.erase(final_str.size() - 1); // remove trailing comma
}

bool isStringDigit(const std::string &str)
{
    for (char c : str)
    {
        if (!std::isdigit(c))
        {
            return false;
        }
    }
    return true;
}

void GCCInline::dump()
{
    std::cout << "Templates: \n";
    auto count = templated_strs.size();
    for (auto x : templated_strs)
    {
        std::cout << "\t\"" << x << "\"";
        if (count > 1)
        {
            std::cout << ",";
            count--;
        }
        std::cout << std::endl;
    }

    count = output_operands.size();
    std::cout << "Output Operands: ";
    for (auto x : output_operands)
    {
        std::cout << x;
        if (count > 1)
        {
            std::cout << ", ";
            count--;
        }
    }
    std::cout << std::endl;

    count << input_operands.size();
    std::cout << "Input Operands: ";
    for (auto x : input_operands)
    {
        std::cout << x << " ";
        if (count > 1)
        {
            std::cout << ", ";
            count--;
        }
    }
    std::cout << std::endl;

    std::cout << "Clobbers: ";
    for (auto x : clobbers)
    {
        std::cout << x << " ";
    }
    std::cout << std::endl;
}