%option noyywrap
%option nounput
%option debug
%option stack
%option bison-bridge

%{
#include "grammar.tab.hh"
%}

digit           [0-9]
letter          [a-zA-Z]
ident_start     [_a-zA-Z]
ident           [_a-zA-Z0-9]
whitespace      [ \t\r]

%%

"+"             { return PLUS;  }
"-"             { return MINUS; }
"*"             { return MULT;  }
"/"             { return SLASH; }
"("             { return LPAREN; }
")"             { return RPAREN; }
";"             { return SEMICOLON; }
","             { return COMMA; }
"."             { return DOT; }
"="             { return EQ; }
"<"             { return LT; }
">"             { return GT; }
"<="            { return LTE; }
">="            { return GTE; }
"if"            { return IF; }
"else"          { return ELSE; }
"while"         { return WHILE; }
"for"           { return FOR; }

{digit}+        { yylval->ival = atoi(yytext);
                  return NUMBER; }
{ident_start}{ident}*   { yylval->sval = strdup(yytext);
                          return IDENT; }

{whitespace}    { }
\n              { }
