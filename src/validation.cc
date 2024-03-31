#include "validation.h"
#include "ast.h"
#include <algorithm>
#include <unordered_map>

void validate_ast(InlineAsm &node)
{
    try
    {
        validate_clobber_abis(node);
        validate_options(node);
        validate_registers(node);
    }
    catch (std::runtime_error &e)
    {
        throw std::runtime_error("Error in validation: " + std::string(e.what()));
    }
}

void validate_clobber_abis(InlineAsm &node)
{
    //
}

void validate_options(InlineAsm &node)
{
    auto options = node.options;

    std::unordered_map<AsmOption, bool> has_option = {
        {AsmOption::PURE, false},
        {AsmOption::NOMEM, false},
        {AsmOption::READONLY, false},
        {AsmOption::PRESERVES_FLAGS, false},
        {AsmOption::NORETURN, false},
        {AsmOption::NOSTACK, false},
        {AsmOption::ATT_SYNTAX, false},
        {AsmOption::RAW, false},
        {AsmOption::MAY_UNWIND, false},
    };

    for (auto option : options)
    {
        has_option[option] = true;
    }

    if (has_option[AsmOption::READONLY] && has_option[AsmOption::NOMEM])
    {
        throw std::runtime_error("Nomem and readonly are mutually exclusive");
    }

    if (has_option[AsmOption::PURE])
    {
        if (!has_option[AsmOption::READONLY] && !has_option[AsmOption::NOMEM])
        {
            throw std::runtime_error("Pure requires readonly or nomem");
        }
    }
}

void validate_registers(InlineAsm &node)
{
    //
}
