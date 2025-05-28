%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int yylex();
extern FILE *yyin;

void yyerror(const char *s);

char* strndup(const char *s, size_t n) {
    char *result = malloc(n + 1);
    if (!result) return NULL;
    memcpy(result, s, n);
    result[n] = '\0';
    return result;
}

typedef enum { TYPE_INT, TYPE_STRING } VarType;

typedef struct {
    VarType type;
    union {
        int ival;
        char *sval;
    } value;
} Variable;

typedef struct VarEntry {
    char *name;
    Variable var;
    struct VarEntry *next;
} VarEntry;

VarEntry *symbolTable = NULL;

void add_variable(char *name, Variable var) {
    VarEntry *entry = malloc(sizeof(VarEntry));
    entry->name = strdup(name);
    entry->var = var;
    entry->next = symbolTable;
    symbolTable = entry;
}

Variable* get_variable(char *name) {
    VarEntry *entry = symbolTable;
    while (entry) {
        if (strcmp(entry->name, name) == 0) return &entry->var;
        entry = entry->next;
    }
    return NULL;
}

void print_var(char *name) {
    Variable *v = get_variable(name);
    if (!v) {
        printf("Undefined variable: %s\n", name);
        return;
    }
    if (v->type == TYPE_INT)
        printf("%d\n", v->value.ival);
    else
        printf("%s\n", v->value.sval);
}
%}

%union {
    int ival;
    char *sval;
}

%token INT STRING AJAY
%token <sval> ID STRING_LITERAL
%token <ival> NUMBER
%token EQ PLUS MINUS MUL DIV
%token LPAREN RPAREN SEMICOLON ASSIGN COMMA

%left PLUS MINUS
%left MUL DIV

%type <ival> expression

%%
program:
    program statement
    | /* empty */
    ;

statement:
    INT ID ASSIGN expression SEMICOLON {
        Variable v;
        v.type = TYPE_INT;
        v.value.ival = $4;
        add_variable($2, v);
        free($2);
    }
    | STRING ID ASSIGN STRING_LITERAL SEMICOLON {
        Variable v;
        v.type = TYPE_STRING;
        int len = strlen($4);
        char *unquoted = strndup($4 + 1, len - 2);
        v.value.sval = unquoted;
        add_variable($2, v);
        free($2);
        free($4);
    }
    | AJAY LPAREN ajay_print_args RPAREN SEMICOLON
    ;

ajay_print_args:
      STRING_LITERAL {
          int len = strlen($1);
          char *unquoted = strndup($1 + 1, len - 2);
          printf("%s\n", unquoted);
          free(unquoted);
          free($1);
      }
    | STRING_LITERAL COMMA expression {
          int len = strlen($1);
          char *unquoted = strndup($1 + 1, len - 2);
          printf("%s%d\n", unquoted, $3);
          free(unquoted);
          free($1);
      }
    | ID {
          print_var($1);
          free($1);
      }
    | expression {
          printf("%d\n", $1);
      }
    ;

expression:
    NUMBER                { $$ = $1; }
    | ID {
        Variable *v = get_variable($1);
        if (!v || v->type != TYPE_INT) {
            printf("Invalid int variable: %s\n", $1);
            $$ = 0;
        } else {
            $$ = v->value.ival;
        }
        free($1);
    }
    | expression PLUS expression   { $$ = $1 + $3; }
    | expression MINUS expression  { $$ = $1 - $3; }
    | expression MUL expression    { $$ = $1 * $3; }
    | expression DIV expression    { $$ = ($3 != 0) ? ($1 / $3) : 0; }
    | LPAREN expression RPAREN     { $$ = $2; }
    ;
%%

void yyerror(const char *s) {
    fprintf(stderr, "Parse error: %s\n", s);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <sourcefile>\n", argv[0]);
        return 1;
    }

    FILE *f = fopen(argv[1], "r");
    if (!f) {
        perror("Error opening file");
        return 1;
    }

    yyin = f;
    yyparse();
    fclose(f);
    return 0;
}
