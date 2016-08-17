%option noyywrap
%option nounput
%option debug
%option yylineno
%option bison-bridge bison-locations

%{
#include "grammar.tab.h"
#define YY_USER_ACTION yylloc->first_line = yylloc->last_line = yylineno;
%}

digit           [0-9]
nonzerodigit    [1-9]
octaldigit      [0-7]
hexdigit        [0-9a-fA-F]
letter          [a-zA-Z]
ident_start     [_a-zA-Z]
ident           [_a-zA-Z0-9]
whitespace      [ \t\r]
sign            [-+]

unsigned_suffix         [uU]
long_suffix             [lL]
long_long_suffix        (ll|LL)
int_suffix              ({unsigned_suffix}{long_suffix}?|{unsigned_suffix}{long_long_suffix}|{long_suffix}{unsigned_suffix}?|{long_long_suffix}{unsigned_suffix}?)

digit_seq               {digit}+
fractional_const        ({digit_seq}?\.{digit_seq}|{digit_seq}\.)
exponent_part           [eE]{sign}?{digit_seq}
float_suffix            [flFL]
decimal_float           {fractional_const}{exponent_part}?{float_suffix}?

hex_prefix              (Ox|OX)
hex_digit_sequence      {hexdigit}+
digit_sequence          {digit}+
binary_exponent_part    [pP]{sign}?{digit_sequence}
hex_fractional_const    ({hex_digit_sequence}?\.{hex_digit_sequence}|{hex_digit_sequence}\.)
hex_fractional_float    {hex_prefix}{hex_fractional_const}{binary_exponent_part}{float_suffix}?
hex_decimal_float       {hex_prefix}{hex_digit_sequence}{binary_exponent_part}{float_suffix}?
hex_float               ({hex_fractional_float}|{hex_decimal_float})


%%

"..."           { return ELLIPSIS; }
"<<="           { return LSHIFTEQ; }
">>="           { return RSHIFTEQ; }
"&="            { return BANDEQ; }
"^="            { return BXOREQ; }
"|="            { return BOREQ; }
"<<"            { return LSHIFT; }
">>"            { return RSHIFT; }
"<="            { return LTE; }
">="            { return GTE; }
"++"            { return PLUSPLUS;  }
"||"            { return OR; }
"->"            { return ARROW; }
"&&"            { return AND;  }
"!="            { return NEQ; }
"=="            { return EQEQ; }
"="             { return EQ; }
"*="            { return MULTEQ; }
"/="            { return DIVEQ; }
"%="            { return MODEQ; }
"+="            { return PLUSEQ; }
"-="            { return MINUSEQ; }
"+"             { return PLUS;  }
"%"             { return PERCENT;  }
"&"             { return AMPERSAND; }
"?"             { return QMARK; }
"^"             { return CARET; }
"|"             { return PIPE; }
"-"             { return MINUS; }
"*"             { return MULT;  }
"~"             { return TILDE; }
"!"             { return BANG;  }
"/"             { return SLASH; }
"("             { return LPAREN; }
")"             { return RPAREN; }
"{"             { return LBRACE; }
"}"             { return RBRACE; }
"["             { return LBRACKET; }
"]"             { return RBRACKET; }
";"             { return SEMICOLON; }
":"             { return COLON; }
","             { return COMMA; }
"."             { return DOT; }
"<"             { return LT; }
">"             { return GT; }
"auto"          { return AUTO; }
"break"         { return BREAK; }
"case"          { return CASE; }
"char"          { return CHAR; }
"const"         { return CONST; }
"continue"      { return CONTINUE; }
"default"       { return DEFAULT; }
"double"        { return DOUBLE; }
"do"            { return DO; }
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

{nonzerodigit}{digit}*{int_suffix}?     { yylval->sval = strdup(yytext);
                                          return INT_LIT; }
0{octaldigit}*{int_suffix}?             { yylval->sval = strdup(yytext);
                                          return OCTAL_LIT; }
("Ox"|"OX"){hexdigit}+{int_suffix}?     { yylval->sval = strdup(yytext);
                                          return HEX_LIT; }
{decimal_float}                         { yylval->sval = strdup(yytext);
                                          return FLOAT_LIT; }
{hex_float}                             { yylval->sval = strdup(yytext);
                                          return HEX_FLOAT_LIT; }
{ident_start}{ident}*                   { yylval->sval = strdup(yytext);
                                          return IDENT; }
L?'(\\.|[^\\'\n])+'                       { yylval->sval = strdup(yytext);
                                          return CHAR_LIT; }
L?\"(\\.|[^\\"\n])*\"                     { yylval->sval = strdup(yytext);
                                          return STRING_LIT; }

{whitespace}    { }
\n              { }
