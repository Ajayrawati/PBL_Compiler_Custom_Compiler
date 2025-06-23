#include "list.h"
#include "pretty_print.h"
#include "semant.h"
#include "interp.h"  // ✅ include this
#include <stdio.h>

extern int yyparse();

struct list* program = NULL; 
int main() {
    if (yyparse() == 0 && program != NULL) {
        // Debug print
        // pretty_print(program->head->data);

        analyze(program);             // Semantic checks
        interpret_program(program);   // 🧠 Interpreter (runs your code)
    } else {
        fprintf(stderr, "Parsing failed or program is empty.\n");
    }
    return 0;
}
