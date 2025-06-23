#ifndef SYMTAB_H
#define SYMTAB_H

struct AST;   // Forward declaration for AST usage in symbol table

// Symbol node structure (for identifiers: variables, functions, etc.)
struct symnode {
    char* name;               // Identifier name
    struct AST* node;         // Associated AST node (declaration or definition)
    struct symnode* next;
    struct symnode* prev;
};

// Symbol table structure
struct symtab {
    struct symtab* prevscope; // For scope chaining (nested blocks/functions)
    struct symnode* head;
    struct symnode* tail;
};

// Scope management
struct symtab* enterscope(struct symtab* current);       // Enter new scope
struct symtab* exitscope(struct symtab* current);        // Exit current scope

// Symbol lookup
struct AST* lookup(struct symtab* table, char* name);    // Search through scopes
struct AST* probe(struct symtab* table, char* name);     // Search only current scope

// Symbol addition
struct symtab* add_symbol(struct symtab* table, char* name, struct AST* node); // Add new symbol

#endif
