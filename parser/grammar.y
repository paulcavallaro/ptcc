%{
#include <stdio.h>
#include <string.h>
#include "grammar.tab.h"

#define YY_DECL int yylex(YYSTYPE * yylval_param, YYLTYPE * yylloc_param)
YY_DECL;
extern FILE *yyin;

void yyerror(YYLTYPE * yylloc, const char* str) {
    fprintf(stderr, "error: %s\n", str);
}

%}

%define api.pure full
%locations

%union value
{
    const char* sval;
    int ival;
}

%token          END 0 "end of file"
%token          COMMA ","
%token          DOT "."
%token          EQ "="
%token          GT ">"
%token          GTE ">="
%token  <sval>  IDENT
%token          LBRACE "{"
%token          LPAREN "("
%token          LT "<"
%token          LTE "<="
%left           MINUS "-"
%left           MULT "*"
%token  <ival>  NUMBER
%left           PLUS "+"
%token          RBRACE "}"
%token          RPAREN ")"
%token          SEMICOLON ";"
%token          SLASH "/"
%token          AUTO
%token          BREAK
%token          CASE
%token          CHAR
%token          CONST
%token          CONTINUE
%token          DEFAULT
%token          DO
%token          DOUBLE
%token          ELSE
%token          ENUM
%token          EXTERN
%token          FLOAT
%token          FOR
%token          GOTO
%token          IF
%token          INLINE
%token          INT
%token          LONG
%token          REGISTER
%token          RESTRICT
%token          RETURN
%token          SHORT
%token          SIGNED
%token          SIZEOF
%token          STATIC
%token          STRUCT
%token          SWITCH
%token          TYPEDEF
%token          UNION
%token          UNSIGNED
%token          VOID
%token          VOLATILE
%token          WHILE
%token          _BOOL
%token          _COMPLEX
%token          _IMAGINARY

%%

file:           stmt_list;

stmt_list:      stmt
                | stmt_list SEMICOLON stmt
                | ;

stmt:           if_stmt;

if_stmt:        IF LPAREN bool_expr RPAREN LBRACE stmt_list RBRACE
                | IF LPAREN bool_expr RPAREN LBRACE stmt_list RBRACE ELSE LBRACE stmt_list RBRACE
                | IF LPAREN bool_expr RPAREN stmt;

bool_expr:      expr;

expr:   number
        | IDENT
        | expr op expr
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

