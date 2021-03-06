%option noyywrap
%option nounput
%option debug
%option yylineno
%option reentrant
%option header-file="flex.h"
%option extra-type="ptcc::parser::Parser *"
%option bison-bridge bison-locations

%e  1019
%p  2807
%n  371
%k  284
%a  1213
%o  1117

O   [0-7]
D   [0-9]
NZ  [1-9]
L   [a-zA-Z_]
A   [a-zA-Z_0-9]
H   [a-fA-F0-9]
HP  (0[xX])
E   ([Ee][+-]?{D}+)
P   ([Pp][+-]?{D}+)
FS  (f|F|l|L)
IS  (((u|U)(l|L|ll|LL)?)|((l|L|ll|LL)(u|U)?))
CP  (u|U|L)
SP  (u8|u|U|L)
ES  (\\([\'\"\?\\abfnrtv]|[0-7]{1,3}|x[a-fA-F0-9]+))
WS  [ \t\v\n\f]

%{

#define YYSTYPE ptcc::parser::Token
#define YY_USER_ACTION yylloc->first_line = yylloc->last_line = yylineno;

#include <stdio.h>
#include "parser/scanner.h"
#include "parser/parser.h"
#include "parser/grammar.tab.hpp"

extern void yyerror(YYLTYPE * yylloc, const char *);  /* prints grammar violation message */

int attribute_paren_count{0};
int asm_paren_count{0};

%}

%x IN_COMMENT
%x IN_ATTRIBUTES
%x IN_ASM

%%
<INITIAL>"/*"                           { BEGIN(IN_COMMENT); }
<IN_COMMENT>"*/"                        { BEGIN(INITIAL); }
<IN_COMMENT>.                           { /* consume comment */ }
"//".*                                  { /* consume //-comment */ }
"#".*                                   { /* consume #-pragma */ }


<IN_ATTRIBUTES>")"                      { attribute_paren_count--; if (attribute_paren_count == 0) BEGIN(INITIAL); }
<IN_ATTRIBUTES>"("                      { attribute_paren_count++; }
<IN_ATTRIBUTES>.                        { /* consume attributes, ignore for now */ }

<IN_ASM>")"                             { asm_paren_count--; if (asm_paren_count == 0) BEGIN(INITIAL); }
<IN_ASM>"("                             { asm_paren_count++; }
<IN_ASM>.                               { /* consume asm, ignore for now */ }

"auto"					{ return(AUTO); }
"break"					{ return(BREAK); }
"case"					{ return(CASE); }
"char"					{ return(CHAR); }
"const"					{ return(CONST); }
"continue"				{ return(CONTINUE); }
"default"				{ return(DEFAULT); }
"do"					{ return(DO); }
"double"				{ return(DOUBLE); }
"else"					{ return(ELSE); }
"enum"					{ return(ENUM); }
"extern"				{ return(EXTERN); }
"float"					{ return(FLOAT); }
"for"					{ return(FOR); }
"goto"					{ return(GOTO); }
"if"					{ return(IF); }
"inline"				{ return(INLINE); }
"int"					{ return(INT); }
"long"					{ return(LONG); }
"register"				{ return(REGISTER); }
"restrict"				{ return(RESTRICT); }
"return"				{ return(RETURN); }
"short"					{ return(SHORT); }
"signed"				{ return(SIGNED); }
"sizeof"				{ return(SIZEOF); }
"static"				{ return(STATIC); }
"struct"				{ return(STRUCT); }
"switch"				{ return(SWITCH); }
"typedef"				{ return(TYPEDEF); }
"union"					{ return(UNION); }
"unsigned"				{ return(UNSIGNED); }
"void"					{ return(VOID); }
"volatile"				{ return(VOLATILE); }
"while"					{ return(WHILE); }
"_Alignas"                              { return ALIGNAS; }
"_Alignof"                              { return ALIGNOF; }
"_Atomic"                               { return ATOMIC; }
"_Bool"                                 { return BOOL; }
"_Complex"                              { return COMPLEX; }
"_Generic"                              { return GENERIC; }
"_Imaginary"                            { return IMAGINARY; }
"_Noreturn"                             { return NORETURN; }
"_Static_assert"                        { return STATIC_ASSERT; }
"_Thread_local"                         { return THREAD_LOCAL; }
"__asm"                                 { BEGIN(IN_ASM); }
"__attribute__"                         { BEGIN(IN_ATTRIBUTES); }
"__extension__"                         { /* Ignore __extension__ for now */ }
"__restrict"                            { return(RESTRICT); }
"__restrict__"                          { return(RESTRICT); }
"__func__"                              { return FUNC_NAME; }

{L}{A}*					{
                                                yylval->m_text = std::string(yytext);
                                                auto tok = yyget_extra(yyscanner)->check_type(yytext);
                                                yylval->m_token = tok;
                                                return tok;
                                        }

{HP}{H}+{IS}?				{
                                                return I_CONSTANT;
                                        }
{NZ}{D}*{IS}?                           {
                                                yylval->m_text = yytext;
                                                return I_CONSTANT;
                                        }
"0"{O}*{IS}?				{ return I_CONSTANT; }
{CP}?"'"([^'\\\n]|{ES})+"'"		{
                                                yylval->m_text = yytext;
                                                return I_CONSTANT;
                                        }

{D}+{E}{FS}?				{ return F_CONSTANT; }
{D}*"."{D}+{E}?{FS}?			{
                                                yylval->m_text = yytext;
                                                return F_CONSTANT;
                                        }
{D}+"."{E}?{FS}?			{
                                                yylval->m_text = yytext;
                                                return F_CONSTANT;
                                        }
{HP}{H}+{P}{FS}?			{
                                                yylval->m_text = yytext;
                                                return F_CONSTANT;
                                        }
{HP}{H}*"."{H}+{P}{FS}?			{
                                                yylval->m_text = yytext;
                                                return F_CONSTANT;
                                        }
{HP}{H}+"."{P}{FS}?			{
                                                yylval->m_text = yytext;
                                                return F_CONSTANT;
                                        }

({SP}?\"([^"\\\n]|{ES})*\"{WS}*)+	{
                                                yylval->m_text = yytext;
                                                return STRING_LITERAL;
                                        }

"..."					{ return ELLIPSIS; }
">>="					{ return RIGHT_ASSIGN; }
"<<="					{ return LEFT_ASSIGN; }
"+="					{ return ADD_ASSIGN; }
"-="					{ return SUB_ASSIGN; }
"*="					{ return MUL_ASSIGN; }
"/="					{ return DIV_ASSIGN; }
"%="					{ return MOD_ASSIGN; }
"&="					{ return AND_ASSIGN; }
"^="					{ return XOR_ASSIGN; }
"|="					{ return OR_ASSIGN; }
">>"					{ return RIGHT_OP; }
"<<"					{ return LEFT_OP; }
"++"					{ return INC_OP; }
"--"					{ return DEC_OP; }
"->"					{ return PTR_OP; }
"&&"					{ return AND_OP; }
"||"					{ return OR_OP; }
"<="					{ return LE_OP; }
">="					{ return GE_OP; }
"=="					{ return EQ_OP; }
"!="					{ return NE_OP; }
";"					{ return ';'; }
("{"|"<%")				{ return '{'; }
("}"|"%>")				{ return '}'; }
","					{ return ','; }
":"					{ return ':'; }
"="					{ return '='; }
"("					{ return '('; }
")"					{ return ')'; }
("["|"<:")				{ return '['; }
("]"|":>")				{ return ']'; }
"."					{ return '.'; }
"&"					{ return '&'; }
"!"					{ return '!'; }
"~"					{ return '~'; }
"-"					{ return '-'; }
"+"					{ return '+'; }
"*"					{ return '*'; }
"/"					{ return '/'; }
"%"					{ return '%'; }
"<"					{ return '<'; }
">"					{ return '>'; }
"^"					{ return '^'; }
"|"					{ return '|'; }
"?"					{ return '?'; }

{WS}+					{ /* whitespace separates tokens */ }
.					{ /* discard bad characters */ }

%%
