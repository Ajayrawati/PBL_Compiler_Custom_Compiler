# 🔧 Custom C-like Language Compiler

This project implements a **compiler for a simplified C-like language**, supporting variables, arithmetic, control structures, functions, arrays, print statements, and basic interpretation. The compiler is built using **Flex**, **Bison**, and **C**.

## 🧠 Features

- **Lexical Analysis** with Flex
- **Parsing** using Bison (Yacc)
- **AST Construction**
- **Semantic Analysis**
- **Scope-aware Symbol Table**
- **Interpreter for execution**
- **Support for:**
  - Integer and string types
  - Arithmetic and boolean expressions
  - `if`, `else`, `while`, `return`, `print`
  - Functions with parameters
  - Arrays
  - String literals and print formatting

---

## 📂 Project Structure

| File              | Purpose                                          |
|-------------------|--------------------------------------------------|
| `lexer.l`         | Flex file: tokenizes the source code             |
| `parser.y`        | Bison file: defines grammar rules and AST nodes  |
| `ast.h/.c`        | AST node definitions and constructors            |
| `symtab.h/.c`     | Symbol table with scoping support                |
| `semant.h/.c`     | Semantic analysis: type checks, scope validation |
| `interp.c`        | Interpreter: executes the program from AST       |
| `utils.c`         | Helper functions                                 |
| `main.c`          | Entry point: compiles and interprets input       |
| `list.c`          | Linked list used for AST and argument lists      |

---

## ✅ Example Program

```c
int main() {
    int i, n;
    n = 10;
    i = 1;

    while (i <= n) {
        if (i % 2 != 0) {
            print(i);
        }
        i = i + 1;
    }

    return;
}


## How to Execute 

For Windows :

bioson -d parser.y
flex lexer.l
gcc -o compiler lex.yy.c parser.tab.c ast_cons.c list.c symtab.c semant.c  pretty_print.c main.c interp.c utils.c
Get-Content test/gcd.txt | ./compiler


