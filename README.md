
```markdown
# 🧠 Custom  Compiler 

This project is a **miniature compiler** for a C-like programming language. It is designed for learning purposes and demonstrates key compiler phases: parsing, semantic analysis, AST construction, interpretation, and code execution.

---

## 📦 Features

### ✅ Language Features Supported:
- **Data Types**: `int`, `void`
- **Variables**: Declaration, assignment
- **Functions**: User-defined with parameters and return values
- **Control Flow**:
  - `if`, `else`
  - `while`
  - `return`
- **Operators**:
  - Arithmetic: `+`, `-`, `*`, `/`, `%`
  - Relational: `<`, `<=`, `>`, `>=`, `==`, `!=`
- **I/O**:
  - `print(...)` statement supporting integer expressions

---

## 📁 File Structure

```

.
├── ast.h / ast\_cons.c        # AST structures and constructors
├── interp.c / interp.h       # Interpreter (walks AST and executes logic)
├── parser.y                  # Bison grammar (defines syntax rules)
├── lexer.l                   # Flex lexer (token definitions)
├── symtab.c / symtab.h       # Symbol table for scoping and declarations
├── semant.c / semant.h       # Semantic analyzer (type checking, scoping)
├── pretty\_print.c            # AST pretty-printer (optional)
├── utils.c                   # Helper functions
├── main.c                    # Compiler entry point
├── list.c / list.h           # Singly linked list utilities
├── test.txt                  # Sample test program
└── README.md                 # This file

````

---

## 🛠️ Building the Compiler

Make sure you have `flex`, `bison`, and `gcc` installed.

### Windows (PowerShell):
```sh
flex lexer.l
bison -d parser.y
gcc -o compiler lex.yy.c parser.tab.c ast_cons.c list.c symtab.c semant.c pretty_print.c main.c interp.c utils.c
````

### Linux/macOS:

```sh
flex lexer.l
bison -d parser.y
gcc -o compiler lex.yy.c parser.tab.c ast_cons.c list.c symtab.c semant.c pretty_print.c main.c interp.c utils.c
```

---

## ▶️ Running the Compiler

Compile and run a test input from a file using:

```sh
Get-Content test.txt | ./compiler     # Windows
cat test.txt | ./compiler             # Linux/macOS
```

If your input is valid, the interpreter will evaluate the program and print results of `print(...)` calls.

---

## ✍️ Sample Test Program

```c
Fibonacci Series Test code
num main() {
    num n;
    num i;
    num t1;
    num t2;
    num nextTerm;

    // Set the number of Fibonacci terms
    n = 10;

    // Initialize first two terms
    t1 = 0;
    t2 = 1;

    i = 1;

    reveal(t1);  // Print first term
    reveal(t2);  // Print second term

    loopwhile (i <= n - 2) {
        nextTerm = t1 + t2;
        reveal(nextTerm);

        // Update previous two terms
        t1 = t2;
        t2 = nextTerm;

        i = i + 1;
    }

    giveback;
}

```

---

## 🧠 Interpreter Behavior

* Functions are stored in a **global function registry**.
* Variable environments use a **flat global scope** per function call.
* On function call:

  * Arguments are evaluated and bound to parameters.
  * Old variable state is backed up and restored after the call.
  * Return values are set using a special variable `__retval`.

---

## 🧪 Supported Tests

| Feature    | Example                                        |
| ---------- | ---------------------------------------------- |
| Arithmetic | `x = 5 + 3 * 2;`                               |
| Loop       | `while (x < 10) { x = x + 1; }`                |
| Condition  | `if (x == y) { print(x); } else { print(y); }` |
| Function   | `int add(int a, int b) { return a + b; }`      |
| Print      | `print(x);`                                    |

---

## 🚫 Known Limitations

* No support for:

  * Strings in expressions (just as literals for `print`)
  * Arrays (partial parsing only)
  * Boolean or float types
  * Recursion or nested functions
* No full return propagation (currently, return values are globalized)
* Variable scope is function-level only (no block scope nesting)

---

## 📌 Goals of the Project

* Understand lexical analysis (Flex)
* Learn grammar parsing (Bison/YACC)
* Construct Abstract Syntax Trees (AST)
* Perform semantic analysis (type checking, scoping)
* Execute programs with an interpreter
* Simulate symbol table management
* Understand compiler phases hands-on

---

## 🙌 Credits

This compiler was built by **Ajay Singh** as part of a hands-on educational project exploring language design, compiler construction, and AST-based interpretation.

---

## 🧩 Future Enhancements (Optional Ideas)

* Support for string manipulation and input()
* File I/O features
* Real function return stack handling
* Bytecode generation or LLVM backend
* Optimization passes

---

