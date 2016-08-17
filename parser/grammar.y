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
%token          ELLIPSIS "..."
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
%token  <sval>  STRING_LIT
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

%start declaration;

identifier:     IDENT;

identifier_opt: /* Nothing */
                | identifier;

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
                | STRING_LIT
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

declaration:    declaration_specifiers init_declarator_list_opt SEMICOLON;

declaration_specifiers_opt:     /* Nothing */
                                | declaration_specifiers;

declaration_specifiers:         storage_class_specifier declaration_specifiers_opt
                                | type_specifier declaration_specifiers_opt
                                | type_qualifier declaration_specifiers_opt
                                | func_specifier declaration_specifiers_opt;

init_declarator_list_opt:       /* Nothing */
                                | init_declarator_list;

init_declarator_list:           init_declarator
                                | init_declarator_list COMMA init_declarator;

init_declarator:                declarator
                                | declarator EQ initializer;

storage_class_specifier:        TYPEDEF
                                | EXTERN
                                | STATIC
                                | AUTO
                                | REGISTER;

type_specifier:         VOID
                        | CHAR
                        | SHORT
                        | INT
                        | LONG
                        | FLOAT
                        | DOUBLE
                        | SIGNED
                        | UNSIGNED
                        | _BOOL
                        | _COMPLEX
                        | struct_or_union_specifier
                        | enum_specifier
                        | typedef_name;

struct_or_union_specifier:      struct_or_union identifier_opt LBRACE struct_declaration_list RBRACE
                                | struct_or_union identifier;

struct_or_union:                STRUCT
                                | UNION;

struct_declaration_list:        struct_declaration
                                | struct_declaration_list struct_declaration;

struct_declaration:             specifier_qualifier_list struct_declarator_list SEMICOLON;

specifier_qualifier_list_opt:   /* Nothing */
                                | specifier_qualifier_list;

specifier_qualifier_list:       type_specifier specifier_qualifier_list_opt
                                | type_qualifier specifier_qualifier_list_opt;

struct_declarator_list:         struct_declarator
                                | struct_declarator_list COMMA struct_declarator;

declarator_opt:                 /* Nothing */
                                | declarator;

struct_declarator:              declarator
                                | declarator_opt COLON constant_expr;

enum_specifier:         ENUM identifier_opt LBRACE enumerator_list RBRACE
                        | ENUM identifier_opt LBRACE enumerator_list COMMA RBRACE
                        | ENUM identifier;

enumerator_list:        enumerator
                        | enumerator_list COMMA enumerator;

enumerator:             enum_constant
                        | enum_constant EQ constant_expr;

type_qualifier:         CONST
                        | RESTRICT
                        | VOLATILE;

func_specifier:         INLINE;

declarator:             pointer_opt direct_declarator;

declarator_opt:         /* Nothing */
                        | declarator;

assignment_expr_opt:    /* Nothing */
                        | assignment_expr;

direct_declarator:      identifier
                        | LPAREN direct_declarator RPAREN
                        | direct_declarator LBRACKET type_qualifier_list_opt assignment_expr_opt RBRACKET
                        | direct_declarator LBRACKET STATIC type_qualifier_list_opt assignment_expr RBRACKET
                        | direct_declarator LBRACKET type_qualifier_list STATIC assignment_expr RBRACKET
                        | direct_declarator LBRACKET type_qualifier_list_opt MULT RBRACKET
                        | direct_declarator LPAREN parameter_type_list RPAREN
                        | direct_declarator LPAREN identifier_list_opt RPAREN;

pointer_opt:            /* Nothing */
                        | pointer;

pointer:                MULT type_qualifier_list_opt
                        | MULT type_qualifier_list_opt pointer;

type_qualifier_list_opt:        /* Nothing */
                                | type_qualifier_list;

type_qualifier_list:    type_qualifier
                        | type_qualifier_list type_qualifier;

parameter_type_list:    parameter_list
                        | parameter_list COMMA ELLIPSIS;

parameter_type_list_opt:        /* Nothing */
                                | parameter_type_list;

parameter_list:         parameter_declaration
                        | parameter_list COMMA parameter_declaration;

parameter_declaration:  declaration_specifiers declarator
                        | declaration_specifiers abstract_declarator_opt;

abstract_declarator_opt:        /* Nothing */
                                | abstract_declarator;

identifier_list_opt:            /* Nothing */
                                | identifier_list;

identifier_list:                identifier
                                | identifier_list COMMA identifier;

type_name:                      specifier_qualifier_list abstract_declarator_opt;

abstract_declarator:            pointer
                                | pointer_opt direct_abstract_declarator;

direct_abstract_declarator_opt:         /* Nothing */
                                        | direct_abstract_declarator

direct_abstract_declarator:     LPAREN abstract_declarator RPAREN
                                | direct_abstract_declarator_opt LBRACKET type_qualifier_list_opt assignment_expr_opt RBRACKET
                                | direct_abstract_declarator_opt LBRACKET STATIC type_qualifier_list_opt assignment_expr RBRACKET
                                | direct_abstract_declarator_opt LBRACKET type_qualifier_list STATIC assignment_expr RBRACKET
                                | direct_abstract_declarator_opt LBRACKET MULT RBRACKET
                                | direct_abstract_declarator_opt LBRACKET parameter_type_list_opt RBRACKET;

typedef_name:   identifier;

initializer:    assignment_expr
                | LBRACE initializer_list RBRACE
                | LBRACE initializer_list COMMA RBRACE;

initializer_list:       designation_opt initializer
                        | initializer_list COMMA designation_opt initializer;

designation_opt:        /* Nothing */
                        | designation;

designation:            designator_list EQ;

designator_list:        designator
                        | designator_list designator;

designator:             LBRACKET constant_expr RBRACKET
                        | DOT identifier;


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

