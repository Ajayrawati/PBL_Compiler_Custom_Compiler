#ifndef AST_H
#define AST_H

// Add an actual enum type name
typedef enum {
    BE_LT, BE_LTE,
    BE_GT, BE_GTE,
    BE_EQ, BE_NEQ,
    BE_ADD, BE_SUB,
    BE_MUL, BE_DIV,

    K_NUM,       // numeric literal
    K_STRING,    // string literal
    K_CALL,      // function call

    E_ASSIGN,
    VAR_NAME,
    ARRAY_ACCESS,
    AST_RETURN,
    AST_WHILE,
    AST_IF_THEN,
    COMPOUND,
    PARAM,
    FUNCTION,
    VAR_DECL,
    AST_PRINT ,
    BE_MOD    // new: print statement
} ASTKind;

struct list; // forward declaration

typedef struct AST AST;
struct AST
{
    int lineno_;
    int kind_; // one of ASTKind
    int type_; // TYPE_INT, TYPE_STRING, etc.
    struct symtab* M; // method symbol table
    struct symtab* V; // variable symbol table

    union {
        int val_;          // for K_NUM
        char* str_val_;    // for K_STRING

        struct {
            AST* lhs_;
            AST* rhs_;
        } be;

        struct {
            char* name_;
            struct list* args_;
        } call;

        struct {
            AST* to_;
            AST* expr_;
        } assign;

        struct {
            char* name_;
        } variable;

        struct {
            char* name_;
            AST* expr_;
        } access;

        struct {
            AST* expr_;
        } ret;

        struct {
            AST* expr_;
            AST* stmt_;
        } while_loop;

        struct {
            AST* expr_;
            AST* if_stmt_;
            AST* else_stmt_;
        } if_then;

        struct {
            struct list* decls_;
            struct list* stmts_;
        } compound;

        struct {
            int type_spec_;
            char* name_;
            int ref_;
        } param;

        struct {
            int ret_type_;
            char* name_;
            struct list* params_;
            AST* body_;
        } function;

        struct {
            int type_spec_;
            char* name_;
            int size_;
            int ref_;
        } var_decl;

        struct {
            struct list* args_;  // args to print
        } print_stmt;

    } d;
};

#endif
