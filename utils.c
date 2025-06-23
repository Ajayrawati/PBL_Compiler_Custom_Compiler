#include "utils.h"
#include "list.h"
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static void dump_cur_(struct symtab* cur, int indent)
{
    for (struct symnode* tmp = cur->head; tmp != NULL; tmp = tmp->next)
    {
        indentation(indent);
        printf("%s\n", tmp->name);
    }
}

void dump_symtab(struct symtab* stab)
{
    if (stab == NULL) return;

    int indent = 0;
    for (struct symtab* tmp = stab; tmp != NULL; tmp = tmp->prevscope)
    {
        indentation(indent);
        printf("scope begins:\n");
        dump_cur_(tmp, indent + 2);
        indent += 4;
    }
}

void indentation(int indent)
{
    for (; indent > 0; --indent)
        putchar(' ');
}

void emit1(const char* s1)
{
    printf("\t%s\n", s1);
}

void emit2(const char* s1, const char* s2)
{
    printf("\t%s %s\n", s1, s2);
}

void emit3(const char* s1, const char* s2, const char* s3)
{
    printf("\t%s %s, %s\n", s1, s2, s3);
}

void emit_label(const char* name)
{
    printf("%s:\n", name);
}

char* new_label()
{
    static int i = 0;
    char buf[20];
    sprintf(buf, ".L%d", i++);
    char* label = (char*)malloc(strlen(buf) + 1);
    strcpy(label, buf);
    return label;
}

void emit_string(const char* label, const char* str)
{
    printf("\t.section .rodata\n");
    printf("%s:\n", label);
    printf("\t.asciz \"%s\"\n", str);
    printf("\t.text\n");
}

void emit_comment(const char* text)
{
    printf("\t# %s\n", text);
}
