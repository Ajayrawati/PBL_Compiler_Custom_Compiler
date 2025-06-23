#include "interp.h"
#include "ast.h"
#include "type.h"
#include "ast_cons.h"
#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"

// Basic symbol table for variable simulation
#define MAX_VARS 100

typedef struct {
    char *name;
    int is_array;
    int value;
    int array_values[100];  // basic array support
} Var;


static Var vars[MAX_VARS];
static int var_count = 0;
static void set_array_elem(const char *name, int index, int value) {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(vars[i].name, name) == 0 && vars[i].is_array) {
            vars[i].array_values[index] = value;
            return;
        }
    }
}

static int get_array_elem(const char *name, int index) {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(vars[i].name, name) == 0 && vars[i].is_array) {
            return vars[i].array_values[index];
        }
    }
    return 0;
}

static int get_var(const char *name)
{
    for (int i = 0; i < var_count; i++)
    {
        if (strcmp(vars[i].name, name) == 0)
            return vars[i].value;
    }
    return 0; // default value if not found
}

static void set_var(const char *name, int value)
{
    for (int i = 0; i < var_count; i++)
    {
        if (strcmp(vars[i].name, name) == 0)
        {
            vars[i].value = value;
            return;
        }
    }
    vars[var_count].name = strdup(name);
    vars[var_count].value = value;
    var_count++;
}

int interpret_expr(AST *expr)
{
    switch (expr->kind_)
    {
    case K_NUM:
        return expr->d.val_;
    case E_ASSIGN:
    {
        int val = interpret_expr(expr->d.assign.expr_);
        const char *name = expr->d.assign.to_->d.variable.name_;
        set_var(name, val);
        return val;
    }

    case VAR_NAME:
        return get_var(expr->d.variable.name_);
    case K_CALL:
    {
        AST *f = lookup(expr->M, expr->d.call.name_);
        if (!f || f->kind_ != FUNCTION)
        {
            fprintf(stderr, "Unknown or unsupported function call: %s\n", expr->d.call.name_);
            return 0;
        }

        // Step 1: Evaluate arguments
        struct node *arg_node = expr->d.call.args_->head;
        struct node *param_node = f->d.function.params_->head;

        // Step 2: Create a new scope for the function call
        struct symtab *V = enterscope(f->V);

        // Step 3: Bind arguments to parameters
        while (arg_node && param_node)
        {
            int arg_val = interpret_expr(arg_node->data);
            char *name = param_node->data->d.param.name_;
            AST *local_var = var_decl(TYPE_INT, name, 0, 0); // Create local int var
            add_symbol(V, name, local_var);                  // Add to symbol table

            // Create assignment: name = arg_val
            AST *assignment = assign(variable(name), num(arg_val));
            interpret_expr(assignment);

            arg_node = arg_node->next;
            param_node = param_node->next;
        }

        // Step 4: Interpret the function body
        int ret_val = interpret_stmt(f->d.function.body_);

        // Step 5: Exit scope
        V = exitscope(V);

        return ret_val;
    }
    case ARRAY_ACCESS:
        return get_array_elem(expr->d.access.name_, interpret_expr(expr->d.access.expr_));

    case BE_ADD:
        return interpret_expr(expr->d.be.lhs_) + interpret_expr(expr->d.be.rhs_);
    case BE_SUB:
        return interpret_expr(expr->d.be.lhs_) - interpret_expr(expr->d.be.rhs_);
    case BE_MUL:
        return interpret_expr(expr->d.be.lhs_) * interpret_expr(expr->d.be.rhs_);
    case BE_DIV:
        return interpret_expr(expr->d.be.lhs_) / interpret_expr(expr->d.be.rhs_);
    case BE_MOD:
        return interpret_expr(expr->d.be.lhs_) % interpret_expr(expr->d.be.rhs_);
    case BE_LT:
        return interpret_expr(expr->d.be.lhs_) < interpret_expr(expr->d.be.rhs_);
    case BE_LTE:
        return interpret_expr(expr->d.be.lhs_) <= interpret_expr(expr->d.be.rhs_);
    case BE_GT:
        return interpret_expr(expr->d.be.lhs_) > interpret_expr(expr->d.be.rhs_);
    case BE_GTE:
        return interpret_expr(expr->d.be.lhs_) >= interpret_expr(expr->d.be.rhs_);
    case BE_EQ:
        return interpret_expr(expr->d.be.lhs_) == interpret_expr(expr->d.be.rhs_);
    case BE_NEQ:
        return interpret_expr(expr->d.be.lhs_) != interpret_expr(expr->d.be.rhs_);

    default:
        fprintf(stderr, "Unsupported expression kind %d\n", expr->kind_);
        return 0;
    }
}

int interpret_stmt(AST *stmt)
{
    switch (stmt->kind_)
    {
    case COMPOUND:
        if (stmt->d.compound.decls_)
        {
            for (struct node *tmp = stmt->d.compound.decls_->head; tmp; tmp = tmp->next)
                interpret_stmt(tmp->data);
        }
        if (stmt->d.compound.stmts_)
        {
            for (struct node *tmp = stmt->d.compound.stmts_->head; tmp; tmp = tmp->next)
            {
                int result = interpret_stmt(tmp->data);
                // Early return if inner stmt has a return
                if (tmp->data->kind_ == AST_RETURN)
                    return result;
            }
        }
        break;

    case VAR_DECL:
        if (stmt->d.var_decl.size_ > 0)
        {
            vars[var_count].name = strdup(stmt->d.var_decl.name_);
            vars[var_count].is_array = 1;
            memset(vars[var_count].array_values, 0, sizeof(vars[var_count].array_values));
            var_count++;
        }
        else
        {
            set_var(stmt->d.var_decl.name_, 0);
        }
        break;

    case E_ASSIGN:
        if (stmt->d.assign.to_->kind_ == ARRAY_ACCESS)
        {
            set_array_elem(
                stmt->d.assign.to_->d.access.name_,
                interpret_expr(stmt->d.assign.to_->d.access.expr_),
                interpret_expr(stmt->d.assign.expr_));
        }
        else
        {
            set_var(stmt->d.assign.to_->d.variable.name_, interpret_expr(stmt->d.assign.expr_));
        }
        break;

    case AST_PRINT:
        for (struct node *tmp = stmt->d.print_stmt.args_->head; tmp; tmp = tmp->next)
            printf("%d\n", interpret_expr(tmp->data));
        break;

    case AST_IF_THEN:
        if (interpret_expr(stmt->d.if_then.expr_))
        {
            return interpret_stmt(stmt->d.if_then.if_stmt_);
        }
        else if (stmt->d.if_then.else_stmt_)
        {
            return interpret_stmt(stmt->d.if_then.else_stmt_);
        }
        break;

    case AST_WHILE:
        while (interpret_expr(stmt->d.while_loop.expr_))
        {
            int result = interpret_stmt(stmt->d.while_loop.stmt_);
            // Check for return inside loop
            if (stmt->d.while_loop.stmt_->kind_ == AST_RETURN)
                return result;
        }
        break;

    case FUNCTION:
        return interpret_stmt(stmt->d.function.body_);

    case AST_RETURN:
        if (stmt->d.ret.expr_ != NULL)
            return interpret_expr(stmt->d.ret.expr_);
        else
            return 0;

    default:
        fprintf(stderr, "Unsupported statement kind %d\n", stmt->kind_);
        break;
    }

    return 0; // default return if no return statement was hit
}

void interpret_program(struct list *prog)
{
    for (struct node *tmp = prog->head; tmp != NULL; tmp = tmp->next)
        interpret_stmt(tmp->data);
}
