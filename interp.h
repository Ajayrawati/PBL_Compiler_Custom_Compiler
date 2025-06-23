#ifndef INTERP_H
#define INTERP_H

#include "ast.h"
#include "list.h"

void interpret_program(struct list* prog);
int interpret_expr(AST* expr);
int interpret_stmt(AST *stmt); 

#endif
