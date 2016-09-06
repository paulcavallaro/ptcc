%code requires{
// This seems to be required to be put into the bison header file otherwise
// yyscan_t isn't defined in the definition of yyparse in the header file.
#ifndef YY_TYPEDEF_YY_SCANNER_T
#define YY_TYPEDEF_YY_SCANNER_T
typedef void *yyscan_t;
#endif
}

%{
// Enable bison debugging capabilities
#define YYDEBUG 1

#include <cstdlib>
#include <memory>
#include <stdio.h>
#include "scanner.h"
#include "parser.h"
#include "grammar.tab.hpp"
#include "flex.h"

#define YYSTYPE ptcc::parser::Token

extern int yylex (YYSTYPE * yylval_param, YYLTYPE * yylloc_param, yyscan_t scanner);
int yylex (YYSTYPE * yylval_param, YYLTYPE * yylloc_param, ptcc::parser::Parser * _p, yyscan_t scanner) {
    return yylex(yylval_param, yylloc_param, scanner);
}
void yyerror(YYLTYPE * yylloc, ptcc::parser::Parser *_p, yyscan_t scanner, const char* str) {
    fprintf(stderr, "error: %s at line %d column %d\n", str,
            yylloc->first_line, yylloc->first_column);
}

using namespace ptcc::parser;

%}

%define api.pure full
%locations
%lex-param {ptcc::parser::Parser *_p}
%parse-param {ptcc::parser::Parser *_p}
%lex-param {yyscan_t scanner}
%parse-param {yyscan_t scanner}


%token	IDENTIFIER I_CONSTANT F_CONSTANT STRING_LITERAL FUNC_NAME SIZEOF
%token	PTR_OP INC_OP DEC_OP LEFT_OP RIGHT_OP LE_OP GE_OP EQ_OP NE_OP
%token	AND_OP OR_OP MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN ADD_ASSIGN
%token	SUB_ASSIGN LEFT_ASSIGN RIGHT_ASSIGN AND_ASSIGN
%token	XOR_ASSIGN OR_ASSIGN
%token	TYPEDEF_NAME ENUMERATION_CONSTANT

%token	TYPEDEF EXTERN STATIC AUTO REGISTER INLINE
%token	CONST RESTRICT VOLATILE
%token	BOOL CHAR SHORT INT LONG SIGNED UNSIGNED FLOAT DOUBLE VOID
%token	COMPLEX IMAGINARY
%token	STRUCT UNION ENUM ELLIPSIS

%token	CASE DEFAULT IF ELSE SWITCH WHILE DO FOR GOTO CONTINUE BREAK RETURN

%token	ALIGNAS ALIGNOF ATOMIC GENERIC NORETURN STATIC_ASSERT THREAD_LOCAL

%start translation_unit

%%

primary_expression
        : IDENTIFIER            {
                                        fprintf(stderr, "Primary Expression of IDENTIFIER: %s\n", $1.m_text.c_str());
                                        $$.m_token = IDENTIFIER;
                                        $$.m_text = $1.m_text;
                                }
        | constant              {
                                        fprintf(stderr, "Primary Expression of Constant: %s\n", $1.m_text.c_str());
                                        $$.m_token = $1.m_token;
                                        $$.m_text = $1.m_text;
                                }
        | string
        | '(' expression ')'
        | generic_selection
        ;

constant
        : I_CONSTANT            {
                                        fprintf(stderr, "Constant of I_CONSTANT: %s\n", $1.m_text.c_str());
                                        $$.m_text = $1.m_text;
                                        $$.m_constant = std::make_shared<Constant>();
                                        $$.m_constant->m_type = ConstantType::Int64;
                                        $$.m_constant->m_text = $1.m_text;
                                        $$.m_constant->m_ivalue = atoi($1.m_text.c_str());
                                 } /* includes character_constant */
        | F_CONSTANT            {
                                        fprintf(stderr, "Constant of F_CONSTANT: %s\n", $1.m_text.c_str());
                                        $$.m_text = $1.m_text;
                                        $$.m_constant = std::make_shared<Constant>();
                                        $$.m_constant->m_type = ConstantType::Double;
                                        $$.m_constant->m_text = $1.m_text;
                                        $$.m_constant->m_dvalue = atof($1.m_text.c_str());
                                }
        | ENUMERATION_CONSTANT  /* after it has been defined as such */
        ;

enumeration_constant            /* before it has been defined as such */
        : IDENTIFIER
        ;

string
        : STRING_LITERAL        {
            fprintf(stderr, "String of STRING_LITERAL: %s\n", $1.m_text.c_str());
            $$ = $1;
        }
        | FUNC_NAME
        ;

generic_selection
        : GENERIC '(' assignment_expression ',' generic_assoc_list ')'
        ;

generic_assoc_list
        : generic_association
        | generic_assoc_list ',' generic_association
        ;

generic_association
        : type_name ':' assignment_expression
        | DEFAULT ':' assignment_expression
        ;

postfix_expression
        : primary_expression            {
                                                fprintf(stderr, "Postfix Expression of Primary Expression: %s\n", $1.m_text.c_str());
                                                $$ = $1;
                                        }
        | postfix_expression '[' expression ']'
        | postfix_expression '(' ')'
        | postfix_expression '(' argument_expression_list ')'
        | postfix_expression '.' IDENTIFIER     {
            fprintf(stderr, "Postfix Expression of Accessing Identifier %s.%s\n", $1.m_text.c_str(), $3.m_text.c_str());
            $$.m_text = $1.m_text + "." + $3.m_text;
        }
        | postfix_expression PTR_OP IDENTIFIER
        | postfix_expression INC_OP
        | postfix_expression DEC_OP
        | '(' type_name ')' '{' initializer_list '}'
        | '(' type_name ')' '{' initializer_list ',' '}'
        ;

argument_expression_list
        : assignment_expression
        | argument_expression_list ',' assignment_expression
        ;

unary_expression
        : postfix_expression            { $$ = $1; }
        | INC_OP unary_expression
        | DEC_OP unary_expression
        | unary_operator cast_expression
        | SIZEOF unary_expression
        | SIZEOF '(' type_name ')'
        | ALIGNOF '(' type_name ')'
        ;

unary_operator
        : '&'
        | '*'
        | '+'
        | '-'
        | '~'
        | '!'
        ;

cast_expression
        : unary_expression              { $$ = $1; }
        | '(' type_name ')' cast_expression
        ;

multiplicative_expression
        : cast_expression               { $$ = $1; }
        | multiplicative_expression '*' cast_expression
        | multiplicative_expression '/' cast_expression
        | multiplicative_expression '%' cast_expression
        ;

additive_expression
        : multiplicative_expression     { $$ = $1; }
        | additive_expression '+' multiplicative_expression     {
                fprintf(stderr, "Additive Expression: %s + %s\n", $1.m_text.c_str(), $3.m_text.c_str());
                $$.m_text = $1.m_text + " + " + $3.m_text;
                auto expr = std::make_shared<BinaryExpression>();
                expr->m_exp1 = $1.m_expr;
                expr->m_exp2 = $3.m_expr;
                expr->m_op = BinOp::Plus;
                $$.m_expr = expr;
        }
        | additive_expression '-' multiplicative_expression
        ;

shift_expression
        : additive_expression   {
            fprintf(stderr, "Shift Expression of Additive Expression: %s\n", $1.m_text.c_str());
            $$ = $1;
        }
        | shift_expression LEFT_OP additive_expression
        | shift_expression RIGHT_OP additive_expression
        ;

relational_expression
        : shift_expression      {
            fprintf(stderr, "Relational Expression of Shift Expression: %s\n", $1.m_text.c_str());
            $$ = $1;
        }
        | relational_expression '<' shift_expression
        | relational_expression '>' shift_expression
        | relational_expression LE_OP shift_expression
        | relational_expression GE_OP shift_expression
        ;

equality_expression
        : relational_expression {
            fprintf(stderr, "Equality Expression of Relational Expression: %s\n", $1.m_text.c_str());
            $$ = $1;
        }
        | equality_expression EQ_OP relational_expression
        | equality_expression NE_OP relational_expression
        ;

and_expression
        : equality_expression   {
            fprintf(stderr, "And Expression of Equality Expression: %s\n", $1.m_text.c_str());
            $$ = $1;
        }
        | and_expression '&' equality_expression
        ;

exclusive_or_expression
        : and_expression        {
            fprintf(stderr, "Exclusive Or Expression of And Expression: %s\n", $1.m_text.c_str());
            $$ = $1;
        }
        | exclusive_or_expression '^' and_expression
        ;

inclusive_or_expression
        : exclusive_or_expression       {
            fprintf(stderr, "Inclusive Or Expression of Exclusive Or Expression: %s\n", $1.m_text.c_str());
            $$ = $1;
        }
        | inclusive_or_expression '|' exclusive_or_expression
        ;

logical_and_expression
        : inclusive_or_expression       {
            fprintf(stderr, "Logical And Expression of Inclusive Or Expression: %s\n", $1.m_text.c_str());
            $$ = $1;
        }
        | logical_and_expression AND_OP inclusive_or_expression
        ;

logical_or_expression
        : logical_and_expression        {
            fprintf(stderr, "Logical Or Expression or Logical And Expression: %s\n", $1.m_text.c_str());
            $$ = $1;
        }
        | logical_or_expression OR_OP logical_and_expression
        ;

conditional_expression
        : logical_or_expression         {
            fprintf(stderr, "Conditional Expression of Logical Or Expression: %s\n", $1.m_text.c_str());
            $$ = $1;
        }
        | logical_or_expression '?' expression ':' conditional_expression
        ;

assignment_expression
        : conditional_expression        {
            fprintf(stderr, "Assignment Expression of Conditional Expression: %s\n", $1.m_text.c_str());
            $$ = $1;
        }
        | unary_expression assignment_operator assignment_expression    {
            fprintf(stderr, "Assignment Expression of [Unary Expression: %s] [Assignment Op: %s] [Assignment Expression: %s]\n", $1.m_text.c_str(), $2.m_text.c_str(), $3.m_text.c_str());
            $$.m_text = $1.m_text + " " + $2.m_text + " " + $3.m_text;
        }
        ;

assignment_operator
        : '='           {
                $$.m_text = "=";
                $$.m_token = '=';
        }
	| MUL_ASSIGN    {
                $$.m_text = "*=";
                $$.m_token = MUL_ASSIGN;
        }
	| DIV_ASSIGN
	| MOD_ASSIGN
	| ADD_ASSIGN
	| SUB_ASSIGN
	| LEFT_ASSIGN
	| RIGHT_ASSIGN
	| AND_ASSIGN
	| XOR_ASSIGN
	| OR_ASSIGN
	;

expression
	: assignment_expression         {
            fprintf(stderr, "Expression of Assignment Expression: %s\n", $1.m_text.c_str());
            $$ = $1;
        }
	| expression ',' assignment_expression
	;

constant_expression
	: conditional_expression	/* with constraints */
	;

declaration
	: declaration_specifiers ';'
	| declaration_specifiers init_declarator_list ';'
	| static_assert_declaration
	;

declaration_specifiers
        : storage_class_specifier declaration_specifiers        {
            fprintf(stderr, "Found a storage_class_specifier %s with declaration_specifiers %s\n",
                    $1.m_text.c_str(), $2.m_text.c_str());
        }
	| storage_class_specifier
	| type_specifier declaration_specifiers
	| type_specifier
	| type_qualifier declaration_specifiers
	| type_qualifier
	| function_specifier declaration_specifiers
	| function_specifier
	| alignment_specifier declaration_specifiers
	| alignment_specifier
	;

init_declarator_list
	: init_declarator
	| init_declarator_list ',' init_declarator
	;

init_declarator
	: declarator '=' initializer
	| declarator
	;

storage_class_specifier
	: TYPEDEF	/* identifiers must be flagged as TYPEDEF_NAME */
	| EXTERN
	| STATIC
	| THREAD_LOCAL
	| AUTO
	| REGISTER
	;

type_specifier
	: VOID          {
                _p->parseTypeSpecifier($$, VOID);
        }
	| CHAR          {
                _p->parseTypeSpecifier($$, CHAR);
        }
	| SHORT         {
                _p->parseTypeSpecifier($$, SHORT);
        }
	| INT           {
                _p->parseTypeSpecifier($$, INT);
        }
	| LONG          {
                _p->parseTypeSpecifier($$, LONG);
        }
	| FLOAT         {
                _p->parseTypeSpecifier($$, FLOAT);
        }
	| DOUBLE        {
                _p->parseTypeSpecifier($$, DOUBLE);
        }
	| SIGNED        {
                _p->parseTypeSpecifier($$, SIGNED);
        }
	| UNSIGNED      {
                _p->parseTypeSpecifier($$, UNSIGNED);
        }
	| BOOL          {
                _p->parseTypeSpecifier($$, BOOL);
        }
	| COMPLEX       {
                _p->parseTypeSpecifier($$, COMPLEX);
        }
	| IMAGINARY	  	/* non-mandated extension */
	| atomic_type_specifier
	| struct_or_union_specifier
	| enum_specifier
	| TYPEDEF_NAME		/* after it has been defined as such */
	;

struct_or_union_specifier
	: struct_or_union '{' struct_declaration_list '}'
	| struct_or_union IDENTIFIER '{' struct_declaration_list '}'    {
            fprintf(stderr, "Struct or Union Specifier of Identifier + Struct Declaration List: %s %s %s\n", $1.m_token == STRUCT ? "struct" : "union", $2.m_text.c_str(), $3.m_text.c_str());
            if ($1.m_token == STRUCT) {
                _p->parseStructSpecifier($$, $2, $4);
            }
        }
	| struct_or_union IDENTIFIER
	;

struct_or_union
	: STRUCT        {
                $$.m_token = STRUCT;
                $$.m_text = "struct";
        }
	| UNION         {
                $$.m_token = UNION;
                $$.m_text = "union";
        }
	;

struct_declaration_list
	: struct_declaration    {
            _p->parseStructDeclarationList($$, $1);
            _p->resetStructDeclaratorList();
        }
	| struct_declaration_list struct_declaration    {
            _p->parseStructDeclarationList($$, $2);
            _p->resetStructDeclaratorList();
        }
	;

struct_declaration
	: specifier_qualifier_list ';'	/* for anonymous struct/union */
	| specifier_qualifier_list struct_declarator_list ';' {
            _p->parseStructDeclaration($$, $1, $2);
        }
	| static_assert_declaration
	;

specifier_qualifier_list
	: type_specifier specifier_qualifier_list       {
            fprintf(stderr, "Specifier Qualifier List Item Addition of Type Specifier: %s %s\n", $1.m_text.c_str(), $2.m_text.c_str());
            $$.m_text = $1.m_text + " " + $2.m_text;
        }
	| type_specifier       {
            fprintf(stderr, "Specifier Qualifier List Base Item of Type Specifier: %s\n", $1.m_text.c_str());
            $$ = $1;
        }
	| type_qualifier specifier_qualifier_list       {
            fprintf(stderr, "Specifier Qualifier List Item Addition of Type Qualifier: %s %s\n", $1.m_text.c_str(), $2.m_text.c_str());
            $$.m_text = $1.m_text + " " + $2.m_text;
        }
	| type_qualifier       {
            fprintf(stderr, "Specifier Qualifier List Base Item of Type Qualifier: %s\n", $1.m_text.c_str());
            $$ = $1;
        }
	;

struct_declarator_list
	: struct_declarator     {
            _p->parseStructDeclarator($$, $1);
        }
	| struct_declarator_list ',' struct_declarator  {
            _p->parseStructDeclarator($$, $3);
        }
	;

struct_declarator
	: ':' constant_expression
	| declarator ':' constant_expression
	| declarator    {
            $$.m_text = $1.m_text;
        }
	;

enum_specifier
	: ENUM '{' enumerator_list '}'
	| ENUM '{' enumerator_list ',' '}'
	| ENUM IDENTIFIER '{' enumerator_list '}'
	| ENUM IDENTIFIER '{' enumerator_list ',' '}'
	| ENUM IDENTIFIER
	;

enumerator_list
	: enumerator
	| enumerator_list ',' enumerator
	;

enumerator	/* identifiers must be flagged as ENUMERATION_CONSTANT */
	: enumeration_constant '=' constant_expression
	| enumeration_constant
	;

atomic_type_specifier
	: ATOMIC '(' type_name ')'
	;

type_qualifier
	: CONST
	| RESTRICT
	| VOLATILE
	| ATOMIC
	;

function_specifier
	: INLINE
	| NORETURN
	;

alignment_specifier
	: ALIGNAS '(' type_name ')'
	| ALIGNAS '(' constant_expression ')'
	;

declarator
	: pointer direct_declarator
	| direct_declarator     {
                $$ = $1;
        }
	;

direct_declarator
	: IDENTIFIER                            {
            _p->parseDirectDeclaratorId($$, $1);
        }
	| '(' declarator ')'
	| direct_declarator '[' ']'
	| direct_declarator '[' '*' ']'
	| direct_declarator '[' STATIC type_qualifier_list assignment_expression ']'
	| direct_declarator '[' STATIC assignment_expression ']'
	| direct_declarator '[' type_qualifier_list '*' ']'
	| direct_declarator '[' type_qualifier_list STATIC assignment_expression ']'
	| direct_declarator '[' type_qualifier_list assignment_expression ']'
	| direct_declarator '[' type_qualifier_list ']'
	| direct_declarator '[' assignment_expression ']'
	| direct_declarator '(' parameter_type_list ')'
	| direct_declarator '(' ')'
	| direct_declarator '(' identifier_list ')'
	;

pointer
	: '*' type_qualifier_list pointer
	| '*' type_qualifier_list
	| '*' pointer
	| '*'
	;

type_qualifier_list
	: type_qualifier
	| type_qualifier_list type_qualifier
	;


parameter_type_list
	: parameter_list ',' ELLIPSIS
	| parameter_list
	;

parameter_list
	: parameter_declaration
	| parameter_list ',' parameter_declaration
	;

parameter_declaration
	: declaration_specifiers declarator
	| declaration_specifiers abstract_declarator
	| declaration_specifiers
	;

identifier_list
	: IDENTIFIER
	| identifier_list ',' IDENTIFIER
	;

type_name
	: specifier_qualifier_list abstract_declarator
	| specifier_qualifier_list
	;

abstract_declarator
	: pointer direct_abstract_declarator
	| pointer
	| direct_abstract_declarator
	;

direct_abstract_declarator
	: '(' abstract_declarator ')'
	| '[' ']'
	| '[' '*' ']'
	| '[' STATIC type_qualifier_list assignment_expression ']'
	| '[' STATIC assignment_expression ']'
	| '[' type_qualifier_list STATIC assignment_expression ']'
	| '[' type_qualifier_list assignment_expression ']'
	| '[' type_qualifier_list ']'
	| '[' assignment_expression ']'
	| direct_abstract_declarator '[' ']'
	| direct_abstract_declarator '[' '*' ']'
	| direct_abstract_declarator '[' STATIC type_qualifier_list assignment_expression ']'
	| direct_abstract_declarator '[' STATIC assignment_expression ']'
	| direct_abstract_declarator '[' type_qualifier_list assignment_expression ']'
	| direct_abstract_declarator '[' type_qualifier_list STATIC assignment_expression ']'
	| direct_abstract_declarator '[' type_qualifier_list ']'
	| direct_abstract_declarator '[' assignment_expression ']'
	| '(' ')'
	| '(' parameter_type_list ')'
	| direct_abstract_declarator '(' ')'
	| direct_abstract_declarator '(' parameter_type_list ')'
	;

initializer
	: '{' initializer_list '}'
	| '{' initializer_list ',' '}'
	| assignment_expression
	;

initializer_list
	: designation initializer
	| initializer
	| initializer_list ',' designation initializer
	| initializer_list ',' initializer
	;

designation
	: designator_list '='
	;

designator_list
	: designator
	| designator_list designator
	;

designator
	: '[' constant_expression ']'
	| '.' IDENTIFIER
	;

static_assert_declaration
	: STATIC_ASSERT '(' constant_expression ',' STRING_LITERAL ')' ';'
	;

statement
	: labeled_statement
	| compound_statement
	| expression_statement
	| selection_statement
	| iteration_statement
	| jump_statement
	;

labeled_statement
	: IDENTIFIER ':' statement
	| CASE constant_expression ':' statement
	| DEFAULT ':' statement
	;

compound_statement
	: '{' '}'
	| '{'  block_item_list '}'
	;

block_item_list
	: block_item
	| block_item_list block_item
	;

block_item
	: declaration
	| statement
	;

expression_statement
	: ';'
	| expression ';'        {
                                        _p->parseExprStmt($$, $1);
                                }
	;

selection_statement
	: IF '(' expression ')' statement ELSE statement
	| IF '(' expression ')' statement
	| SWITCH '(' expression ')' statement
	;

iteration_statement
	: WHILE '(' expression ')' statement
	| DO statement WHILE '(' expression ')' ';'
	| FOR '(' expression_statement expression_statement ')' statement
	| FOR '(' expression_statement expression_statement expression ')' statement
	| FOR '(' declaration expression_statement ')' statement
	| FOR '(' declaration expression_statement expression ')' statement
	;

jump_statement
	: GOTO IDENTIFIER ';'
	| CONTINUE ';'
	| BREAK ';'
	| RETURN ';'
	| RETURN expression ';'         {
            _p->parseReturnStmt($$, RETURN, $1);
        }
	;

translation_unit
	: external_declaration
	| translation_unit external_declaration
	;

external_declaration
	: function_definition
	| declaration
	;

function_definition
	: declaration_specifiers declarator declaration_list compound_statement
	| declaration_specifiers declarator compound_statement
	;

declaration_list
	: declaration
	| declaration_list declaration
	;

%%

int main(int argc, char** argv) {
    if (argc != 1 && argc != 2) {
        fprintf(stderr, "Usage: lexer [FILE]");
        return 1;
    }
    // Turn on bison debugging for this parse
    // yydebug = 1;
    ptcc::parser::Parser p;
    p.setDebug(true);
    yyscan_t scanner;
    yylex_init(&scanner);
    yylex_init_extra(&p, &scanner);

    FILE* fin = nullptr;
    if (argc == 2) {
        fin = fopen(argv[1], "r");
        if (!fin) {
            fprintf(stderr, "Error: Cannot open file %s\n", argv[1]);
            return 1;
        }
        yyset_in(fin, scanner);
    }

    // Set debugging to true for the scanner;
    // yyset_debug(true, scanner);

    auto res = yyparse(&p, scanner);
    yylex_destroy(scanner);

    if (fin) {
        fclose(fin);
    }
    return res;
}
