#include <stdlib.h>
#include "list.h"

// Create an empty list (nil)
list* nil_list() {
    list* l = (list*)malloc(sizeof(list));
    if (!l) return NULL;
    l->head = NULL;
    l->tail = NULL;
    return l;
}

// Create a list with a single AST node
list* single_list(AST* ast) {
    list* l = (list*)malloc(sizeof(list));
    if (!l) return NULL;

    struct node* n = (struct node*)malloc(sizeof(struct node));
    if (!n) {
        free(l);
        return NULL;
    }

    n->data = ast;
    n->next = NULL;

    l->head = n;
    l->tail = n;

    return l;
}

// Append list l2 to the end of l1
list* append_list(list* l1, list* l2) {
    if (!l1) return l2;
    if (!l2) return l1;

    if (l2->head == NULL) {
        // l2 is empty, nothing to append
        free(l2);
        return l1;
    }

    if (l1->tail) {
        l1->tail->next = l2->head;
    } else {
        // l1 is empty
        l1->head = l2->head;
    }

    l1->tail = l2->tail;
    free(l2); // Only free structure, not the nodes
    return l1;
}
