#ifndef PRETTY_PRINT_H
#define PRETTY_PRINT_H

#include "ast.h"

// Pretty prints the abstract syntax tree including extended kinds:
// - K_STRING for string literals
// - AST_PRINT for print statements
void pretty_print(AST*);

#endif
