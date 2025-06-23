#include "symtab.h"
#include "list.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// symbol node with identifier name and associated AST
static struct symnode* make_symnode(char* name, struct AST* node) {
    struct symnode* temp = (struct symnode*)malloc(sizeof(struct symnode));
    if (!temp) {
        fprintf(stderr, "Error: Failed to allocate memory for symnode.\n");
        exit(EXIT_FAILURE);
    }
    temp->name = name;
    temp->node = node;
    temp->next = NULL;
    temp->prev = NULL;
    return temp;
}

// Create a new empty symbol table
static struct symtab* make_symtab() {
    struct symtab* temp = (struct symtab*)malloc(sizeof(struct symtab));
    if (!temp) {
        fprintf(stderr, "Error: Failed to allocate memory for symtab.\n");
        exit(EXIT_FAILURE);
    }
    temp->prevscope = NULL;
    temp->head = NULL;
    temp->tail = NULL;
    return temp;
}

// Enter a new scope (nested symbol table)
struct symtab* enterscope(struct symtab* cur) {
    struct symtab* temp = make_symtab();
    temp->prevscope = cur;
    return temp;
}

// Exit the current scope and return to the previous one
struct symtab* exitscope(struct symtab* cur) {
    if (cur != NULL) {
        struct symtab* prev = cur->prevscope;
        // Optional: free memory if not reused
        // free(cur);
        return prev;
    }
    return NULL;
}

// Search for a symbol name across all nested scopes (outermost last)
struct AST* lookup(struct symtab* cur, char* name) {
    while (cur != NULL) {
        struct AST* found = probe(cur, name);
        if (found != NULL)
            return found;
        cur = cur->prevscope;
    }
    return NULL;
}

// Search only the current scope for a symbol
struct AST* probe(struct symtab* cur, char* name) {
    for (struct symnode* tmp = cur->head; tmp != NULL; tmp = tmp->next) {
        if (strcmp(tmp->name, name) == 0)
            return tmp->node;
    }
    return NULL;
}

// Add a new symbol to the current symbol table
struct symtab* add_symbol(struct symtab* cur, char* name, struct AST* ast) {
    struct symnode* temp = make_symnode(name, ast);

    if (cur->tail == NULL || cur->head == NULL) {
        cur->head = cur->tail = temp;
    } else {
        cur->tail->next = temp;
        temp->prev = cur->tail;
        cur->tail = temp;
    }

    return cur;
}
