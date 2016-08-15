%{
#include <stdio.h>
#include <string.h>
#include <FlexLexer.h>

extern "C" FILE* yyin;

void yyerror(const char* str) {
    fprintf(stderr, "error: %s\n", str);
}

int yylex() {
    return 0;
}

%}

%union
{
    const char* sval;
    int ival;
}

%token  COMMA
%token  DOT
%token  ELSE
%token  EQ
%token  FOR
%token  GT
%token  GTE
%token  IDENT
%token  IF
%token  LPAREN
%token  LT
%token  LTE
%token  MINUS
%token  MULT
%token  NUMBER
%token  PLUS
%token  RPAREN
%token  SEMICOLON
%token  SLASH
%token  WHILE

%locations

%%

expr:
        number
        | op expr expr
        | LPAREN expr RPAREN;

op:     PLUS
        | MINUS
        | MULT;

number: NUMBER;

%%

int main(int argc, char** argv) {
    if (argc != 1 && argc != 2) {
        fprintf(stderr, "Usage: lexer [FILE]");
        return 1;
    }
    std::ifstream istream;
    if (argc == 2) {
         FILE* fin = fopen(argv[1], "r");
         if (!fin) {
             fprintf(stderr, "Error: Cannot open file %s", argv[1]);
             return 1;
         }
        yyin = fin;
    }
    return yyparse();
}

