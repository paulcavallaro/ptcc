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

%token          QMARK "?"
%token          CARET "^"
%token          COLON ":"
%token          PIPE "|"
%token          OR "||"
%token          LSHIFT "<<"
%token          RSHIFT ">>"
%token          MULTEQ "*="
%token          DIVEQ "/="
%token          MODEQ "%="
%token          PLUSEQ "+="
%token          MINUSEQ "-="
%token          LSHIFTEQ "<<="
%token          RSHIFTEQ ">>="
%token          BANDEQ "&="
%token          BXOREQ "^="
%token          BOREQ "|="
%token          END 0 "end of file"
%token          COMMA ","
%token          DOT "."
%token          PERCENT "%"
%token          NEQ "!="
%token          EQ "="
%token          EQEQ "=="
%token          AMPERSAND "&"
%token          AND "&&"
%token          GT ">"
%token          GTE ">="
%token  <sval>  IDENT
%token          LBRACE "{"
%token          LBRACKET "["
%token          LPAREN "("
%token          LT "<"
%token          LTE "<="
%token          PLUSPLUS "++"
%token          MINUSMINUS "--"
%left           MINUS "-"
%left           MULT "*"
%left           TILDE "~"
%left           BANG "!"
%token  <sval>  INT_LIT
%token  <sval>  OCTAL_LIT
%token  <sval>  HEX_LIT
%token  <sval>  FLOAT_LIT
%token  <sval>  HEX_FLOAT_LIT
%token  <sval>  CHAR_LIT
%left           PLUS "+"
%token          RBRACE "}"
%token          RBRACKET "]"
%token          RPAREN ")"
%token          SEMICOLON ";"
%token          SLASH "/"
%token          ARROW "->"
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

%start expr;

identifier:     IDENT;

constant:       int_constant
                | float_constant
                | enum_constant
                | char_constant;

int_constant:   INT_LIT
                | OCTAL_LIT
                | HEX_LIT;

float_constant: FLOAT_LIT
                | HEX_FLOAT_LIT;

enum_constant:  IDENT;

char_constant:  CHAR_LIT;


primary_expr:   identifier
                | constant
                | string_literal
                | LPAREN expr RPAREN;

postfix_expr:   primary_expr
                | postfix_expr LBRACKET expr RBRACKET
                | postfix_expr LPAREN argument_expr_list_opt RPAREN
                | postfix_expr DOT identifier
                | postfix_expr ARROW identifier
                | postfix_expr PLUSPLUS
                | postfix_expr MINUSMINUS
                | LPAREN type_name RPAREN LBRACE initializer_list RBRACE
                | LPAREN type_name RPAREN LBRACE initializer_list COMMA RBRACE;

argument_expr_list_opt:         /* Nothing */
                                | argument_expr_list;

argument_expr_list:     assignment_expr
                        | argument_expr_list COMMA assignment_expr;

unary_expr:     postfix_expr
                | PLUSPLUS unary_expr
                | MINUSMINUS unary_expr
                | unary_op cast_expr
                | SIZEOF unary_expr
                | SIZEOF LPAREN type_name RPAREN;

unary_op:       AMPERSAND
                | MULT
                | PLUS
                | MINUS
                | TILDE
                | BANG;

cast_expr:      unary_expr
                | LPAREN type_name RPAREN cast_expr;

mult_expr:      cast_expr
                | mult_expr MULT cast_expr
                | mult_expr SLASH cast_expr
                | mult_expr PERCENT cast_expr;

add_expr:       mult_expr
                | add_expr PLUS mult_expr
                | add_expr MINUS mult_expr;

shift_expr:     add_expr
                | shift_expr LSHIFT add_expr
                | shift_expr RSHIFT add_expr;

rel_expr:       shift_expr
                | rel_expr LT shift_expr
                | rel_expr GT shift_expr
                | rel_expr LTE shift_expr
                | rel_expr GTE shift_expr;

eq_expr:        rel_expr
                | eq_expr EQEQ rel_expr
                | eq_expr NEQ rel_expr;

and_expr:       eq_expr
                | and_expr AMPERSAND eq_expr;

excl_or_expr:   and_expr
                | excl_or_expr CARET and_expr;

incl_or_expr:   excl_or_expr
                | incl_or_expr PIPE excl_or_expr;

logical_and_expr:       incl_or_expr
                        | logical_and_expr AND incl_or_expr;

logical_or_expr:        logical_and_expr
                        | logical_or_expr OR logical_and_expr;

cond_expr:      logical_or_expr
                | logical_or_expr QMARK expr COLON cond_expr;

assignment_expr:        cond_expr
                        | unary_expr assignment_op assignment_expr;

assignment_op:          EQ
                        | MULTEQ
                        | DIVEQ
                        | MODEQ
                        | PLUSEQ
                        | MINUSEQ
                        | LSHIFTEQ
                        | RSHIFTEQ
                        | BANDEQ
                        | BXOREQ
                        | BOREQ;

expr:   assignment_expr
        | expr COMMA assignment_expr;

constant_expr:  cond_expr;

// TODO(ptc) Continue with these definitions, right now just placeholders

initializer_list:       IDENT;
string_literal:       IDENT;
type_name:      IDENT;


%%

int main(int argc, char** argv) {
    if (argc != 1 && argc != 2) {
        fprintf(stderr, "Usage: lexer [FILE]");
        return 1;
    }
    if (argc == 2) {
         FILE* fin = fopen(argv[1], "r");
         if (!fin) {
             fprintf(stderr, "Error: Cannot open file %s\n", argv[1]);
             return 1;
         }
         yyin = fin;
    }
    return yyparse();
}

