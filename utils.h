#ifndef UTILS_H
#define UTILS_H

#include "symtab.h"

void dump_symtab(struct symtab* stab);
void indentation(int indent);

void emit1(const char* op);
void emit2(const char* op, const char* arg1);
void emit3(const char* op, const char* arg1, const char* arg2);

void emit_label(const char* label);
char* new_label();

void emit_string(const char* label, const char* str);
void emit_comment(const char* text);

#endif
