#include "translation.h"

GCCInline print_translated_node(InlineAsm &node)
{
    auto gcc_node = GCCInline();
    for (auto x : node.operands)
    {
        translate_reg(x, gcc_node);
    }
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
            gcc_inline.output_operands.push_back("\"=r\"(" + tmp + ")");
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
            auto final_str = "\"" + constraint + "\"(" + expression + ")";
            gcc_inline.input_operands.push_back(final_str);
        }
        if (reg.type == OperandType::OUT)
        {
            auto final_str = "\"=" + constraint + "\"(" + expression + ")";
            gcc_inline.output_operands.push_back(final_str);
        }
        if (reg.type == OperandType::INOUT)
        {
            auto final_str = "\"+" + constraint + "\"(" + expression + ")";
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

void GCCInline::dump()
{
    std::cout << "Templates: ";
    for (auto x : templates)
    {
        std::cout << x << " ";
    }
    std::cout << std::endl;
    std::cout << "Output Operands: ";
    for (auto x : output_operands)
    {
        std::cout << x << " ";
    }
    std::cout << std::endl;
    std::cout << "Input Operands: ";
    for (auto x : input_operands)
    {
        std::cout << x << " ";
    }
    std::cout << std::endl;
    std::cout << "Clobbers: ";
    for (auto x : clobbers)
    {
        std::cout << x << " ";
    }
    std::cout << std::endl;
}