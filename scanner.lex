%{
#include <vector>
#include "nodes.hpp"
#include "parser.tab.hpp"
#include "hw3_output.hpp"
#include <string>
#include <iostream>
%}

%option yylineno
%option noyywrap
string          (\"([^\n\r\"\\]|\\[rnt\"\\]|(\\x[0-7][0-9A-Fa-f]))+\")
invalid_string  (\".*\")
unclosed_string \"{string}[^"]*\"
num             (0|[1-9][0-9]*)
comment         (\/\/[^\r\n]*)
to_ignore       ([\r\n\t\ ]+)
id              ([a-zA-Z][a-zA-Z0-9]*)
relop           (==|!=|<|>|<=|>=)
binop_1         (\*|\/)
binop_2         (\+|\-)

%%

void                            { return VOID; }
int                             { return INT; }
byte                            { return BYTE; }
b                               { return B; }
bool                            { return BOOL; }
and                             { return AND; }
or                              { return OR; }
not                             { return NOT; }
true                            { return TRUE; }
false                           { return FALSE; }
return                          { return RETURN; }
if                              { return IF; }
else                            { return ELSE; }
while                           { return WHILE; }
break                           { return BREAK; }
continue                        { return CONTINUE; }
override                        { return OVERRIDE; }
;                               { return SC; }
,                               { return COMMA; }
\(                              { return LPAREN; }
\)                              { return RPAREN; }
\{                              { return LBRACE; }
\}                              { return RBRACE; }
=                               { return ASSIGN; }
{relop}                         { yylval.str = strdup(yytext); return RELOP; }
{binop_1}                       { yylval.str = strdup(yytext); return BINOP_1; }
{binop_2}                       { yylval.str = strdup(yytext); return BINOP_2; }
{num}                           { yylval.str = strdup(yytext); return NUM; }
{string}                        { yylval.str = strdup(yytext); return STRING; }
{id}                            { yylval.str = strdup(yytext); return ID; }
{comment}                       {}
{to_ignore}                     {}
{invalid_string}                {}
{unclosed_string}               {}
.                               { output::errorLex(yylineno); exit(1); }

%%