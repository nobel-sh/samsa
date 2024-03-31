#ifndef TRANSLATION_H_
#define TRANSLATION_H_

#include <vector>
#include "ast.h"

class GCCInline{
    public:
        std::vector<std::string> output_operands;
        std::vector<std::string> input_operands;
        std::vector<std::string> clobbers;
        std::vector<std::string> templates;
        GCCInline(std::vector<std::string> output_operands, std::vector<std::string> input_operands,
             std::vector<std::string> clobbers, std::vector<std::string> templates)
            : output_operands(output_operands), input_operands(input_operands), 
            clobbers(clobbers), templates(templates) {}
        GCCInline() {}
        void dump();
};

GCCInline print_translated_node(InlineAsm& node);
void translate_reg(Operand& reg, GCCInline& gcc_inline);
std::string give_reg_from_reg_class(std::string reg);


#endif // TRANSLATION_H_
