%{
#include "parser.tab.h"
#include <string.h>
#include <stdlib.h>
%}

%%
"int"           { return INT; }
"string"        { return STRING; }
"ajay"          { return AJAY; }

\"([^\\\"]|\\.)*\"   {
    yylval.sval = strdup(yytext);
    return STRING_LITERAL;
}

[a-zA-Z_][a-zA-Z0-9_]* {
    yylval.sval = strdup(yytext);
    return ID;
}

[0-9]+ {
    yylval.ival = atoi(yytext);
    return NUMBER;
}

"="             { return ASSIGN; }
";"             { return SEMICOLON; }
","             { return COMMA; }
"("             { return LPAREN; }
")"             { return RPAREN; }
"+"             { return PLUS; }
"-"             { return MINUS; }
"*"             { return MUL; }
"/"             { return DIV; }

[ \t\n\r]+      ; // skip whitespace

.               { return yytext[0]; }
%%
int yywrap(){return 1;}