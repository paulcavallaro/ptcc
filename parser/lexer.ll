%option noyywrap
%option nounput
%option debug
%option bison-bridge bison-locations

%{
#include "grammar.tab.h"
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
"{"             { return LBRACE; }
"}"             { return RBRACE; }
";"             { return SEMICOLON; }
","             { return COMMA; }
"."             { return DOT; }
"="             { return EQ; }
"<"             { return LT; }
">"             { return GT; }
"<="            { return LTE; }
">="            { return GTE; }
"auto"          { return AUTO; }
"break"         { return BREAK; }
"case"          { return CASE; }
"char"          { return CHAR; }
"const"         { return CONST; }
"continue"      { return CONTINUE; }
"default"       { return DEFAULT; }
"do"            { return DO; }
"double"        { return DOUBLE; }
"else"          { return ELSE; }
"enum"          { return ENUM; }
"extern"        { return EXTERN; }
"float"         { return FLOAT; }
"for"           { return FOR; }
"goto"          { return GOTO; }
"if"            { return IF; }
"inline"        { return INLINE; }
"int"           { return INT; }
"long"          { return LONG; }
"register"      { return REGISTER; }
"restrict"      { return RESTRICT; }
"return"        { return RETURN; }
"short"         { return SHORT; }
"signed"        { return SIGNED; }
"sizeof"        { return SIZEOF; }
"static"        { return STATIC; }
"struct"        { return STRUCT; }
"switch"        { return SWITCH; }
"typedef"       { return TYPEDEF; }
"union"         { return UNION; }
"unsigned"      { return UNSIGNED; }
"void"          { return VOID; }
"volatile"      { return VOLATILE; }
"while"         { return WHILE; }
"_Bool"         { return _BOOL; }
"_Complex"      { return _COMPLEX; }
"_Imaginary"    { return _IMAGINARY; }
{digit}+        { yylval->ival = atoi(yytext);
                  return NUMBER; }
{ident_start}{ident}*   { yylval->sval = strdup(yytext);
                          return IDENT; }

{whitespace}    { }
\n              { }
