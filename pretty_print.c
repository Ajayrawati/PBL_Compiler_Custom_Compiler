#include "pretty_print.h"
#include "list.h"
#include "type.h"
#include "utils.h"
#include <stdio.h>

static void pprint_(AST* ast, int indent);
static void pprint(AST* ast, int indent);

void pretty_print(AST* ast)
{
    pprint(ast, 0);
}

static void pprint(AST* ast, int indent)
{
    if (ast != NULL)
        pprint_(ast, indent);
}

static void print_binexpr(int indent, const char* rep, AST* ast)
{
    indentation(indent);
    printf("%s\n", rep);
    pprint(ast->d.be.lhs_, indent + 2);
    pprint(ast->d.be.rhs_, indent + 2);
}

static void pprint_(AST* ast, int indent)
{
    switch (ast->kind_)
    {
        case K_NUM:
            indentation(indent);
            printf("%d\n", ast->d.val_);
            break;

        case K_STRING:
            indentation(indent);
            printf("\"%s\"\n", ast->d.str_val_);
            break;

        case BE_LT:  print_binexpr(indent, "<", ast); break;
        case BE_LTE: print_binexpr(indent, "<=", ast); break;
        case BE_GT:  print_binexpr(indent, ">", ast); break;
        case BE_GTE: print_binexpr(indent, ">=", ast); break;
        case BE_EQ:  print_binexpr(indent, "==", ast); break;
        case BE_NEQ: print_binexpr(indent, "!=", ast); break;
        case BE_ADD: print_binexpr(indent, "+", ast); break;
        case BE_SUB: print_binexpr(indent, "-", ast); break;
        case BE_MUL: print_binexpr(indent, "*", ast); break;
        case BE_DIV: print_binexpr(indent, "/", ast); break;

        case K_CALL:
            indentation(indent);
            printf("Call: %s\n", ast->d.call.name_);
            for (struct node* tmp = ast->d.call.args_->head; tmp != NULL; tmp = tmp->next)
                pprint(tmp->data, indent + 2);
            break;

        case E_ASSIGN:
            indentation(indent);
            printf("=\n");
            pprint(ast->d.assign.to_, indent + 2);
            pprint(ast->d.assign.expr_, indent + 2);
            break;

        case ARRAY_ACCESS:
            indentation(indent);
            printf("Array name: %s\n", ast->d.access.name_);
            pprint(ast->d.access.expr_, indent + 2);
            break;

        case VAR_NAME:
            indentation(indent);
            printf("variable name: %s\n", ast->d.variable.name_);
            break;

        case AST_RETURN:
            indentation(indent);
            printf("Return statement\n");
            if (ast->d.ret.expr_ != NULL)
                pprint(ast->d.ret.expr_, indent + 2);
            break;

        case AST_WHILE:
            indentation(indent);
            printf("While statement\n");
            pprint(ast->d.while_loop.expr_, indent + 2);
            pprint(ast->d.while_loop.stmt_, indent + 2);
            break;

        case AST_IF_THEN:
            indentation(indent);
            printf("If statement\n");
            pprint(ast->d.if_then.expr_, indent + 2);
            pprint(ast->d.if_then.if_stmt_, indent + 2);
            if (ast->d.if_then.else_stmt_ != NULL) {
                indentation(indent);
                printf("Else\n");
                pprint(ast->d.if_then.else_stmt_, indent + 2);
            }
            break;

        case COMPOUND:
            indentation(indent);
            printf("Declarations:\n");
            for (struct node* tmp = ast->d.compound.decls_->head; tmp != NULL; tmp = tmp->next)
                pprint(tmp->data, indent + 2);
            indentation(indent);
            printf("Statements:\n");
            for (struct node* tmp = ast->d.compound.stmts_->head; tmp != NULL; tmp = tmp->next)
                pprint(tmp->data, indent + 2);
            break;

        case PARAM:
            indentation(indent);
            printf("%s %s%s\n",
                   ast->d.param.type_spec_ == TYPE_INT ? "int" : "void",
                   ast->d.param.name_,
                   ast->d.param.ref_ ? "[]" : "");
            break;

        case FUNCTION:
            indentation(indent);
            printf("%s %s\n",
                   ast->d.function.ret_type_ == TYPE_INT ? "int" : "void",
                   ast->d.function.name_);
            indentation(indent + 2);
            printf("Parameters:\n");
            for (struct node* tmp = ast->d.function.params_->head; tmp != NULL; tmp = tmp->next)
                pprint(tmp->data, indent + 4);
            pprint(ast->d.function.body_, indent + 2);
            break;

        case VAR_DECL:
            indentation(indent);
            printf("%s %s %d %s\n",
                   ast->d.var_decl.type_spec_ == TYPE_INT ? "int" : "void",
                   ast->d.var_decl.name_,
                   ast->d.var_decl.ref_ ? ast->d.var_decl.size_ : 1,
                   ast->d.var_decl.ref_ ? "[]" : "");
            break;

        case AST_PRINT:
            indentation(indent);
            printf("Print:\n");
            for (struct node* tmp = ast->d.print_stmt.args_->head; tmp != NULL; tmp = tmp->next)
                pprint(tmp->data, indent + 2);
            break;

        default:
            indentation(indent);
            printf("Unknown AST node kind: %d\n", ast->kind_);
            break;
    }
}
