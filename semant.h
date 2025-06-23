#ifndef SEMANT_H
#define SEMANT_H

#include "list.h"

// Perform semantic analysis on the full program AST.
// This includes type checking, scope validation, and
// verifying usage of variables, functions, print statements, and string literals.
void analyze(list* program);

#endif
