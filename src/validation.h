#ifndef VALIDATION_H_
#define VALIDATION_H_

#include "ast.h"

void validate_ast(InlineAsm& node);
void validate_options(InlineAsm& node);
void validate_clobber_abis(InlineAsm& node);
void validate_registers(InlineAsm& node);
void validate_template(InlineAsm& node);

#endif // VALIDATION_H_
