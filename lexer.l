%{
#include "parser.tab.h"
#include <string.h>
%}

%option warn noyywrap noinput nounput yylineno

%%

"print"      { return PRINT; }
"else"       { return ELSE; }
"if"         { return IF; }
"int"        { return INT; }
"return"     { return RETURN; }
"void"       { return VOID; }
"while"      { return WHILE; }

"+"          { return ADD; }
"-"          { return SUB; }
"*"          { return MUL; }
"/"          { return DIV; }
"<="         { return LTE; }
"<"          { return LT; }
">="         { return GTE; }
">"          { return GT; }
"=="         { return EQ; }
"!="         { return NEQ; }
"="          { return ASSIGN; }
";"          { return SEMI; }
","          { return COMMA; }
"("          { return ROUND_OPEN; }
")"          { return ROUND_CLOSE; }
"["          { return SQUARE_OPEN; }
"]"          { return SQUARE_CLOSE; }
"{"          { return CURLY_OPEN; }
"}"          { return CURLY_CLOSE; }
"%"     { return MOD; }
"\"([^\\\"]|\\.)*\""   { yylval.string = strdup(yytext); return STRING_LITERAL; }

[a-zA-Z_][a-zA-Z0-9_]* { yylval.string = strdup(yytext); return ID; }
[0-9]+       { yylval.integer = atoi(yytext); return NUM; }

"//".*       /* ignore single-line comments */
[ \t\r]+     /* skip whitespace */
\n           { ++yylineno; }

.            { fprintf(stderr, "unknown character: %c\n", yytext[0]); }

%%