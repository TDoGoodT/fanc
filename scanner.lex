%{
    #include "analyzer.tab.hpp"
    #include "hw3_output.hpp"
    using namespace output;
%}

%option yylineno
%option noyywrap
string      (\"([^\n\r\"\\]|\\[rnt\"\\])+\")
num         (0|[1-9][0-9]*)
to_ignore   ((\/\/[^\r\n]*[ \r|\n|\r\n]?)|([\r\n\t\ ]+))
id          ([a-zA-Z][a-zA-Z0-9]*)
mult        (\*)
div         (\/)
add         (\+)
sub         (-)
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
;                               { return SC; }
,                               { return COMMA; }
\(                              { return LPAREN; }
\)                              { return RPAREN; }
\{                              { return LBRACE; }
\}                              { return RBRACE; }
(=)                             { return ASSIGN; }
(>)                             { return GT; }
(>=)                             { return GE; }
(<)                             { return LT; }
(<=)                             { return LE; }
(!=)                             { return NE; }
(==)                             { return EQ; }
{mult}                          { return MULT; }
{div}                           { return DIV; }
{sub}                           { return SUB; }
{add}                           { return ADD; }
{num}                           { return NUM; }
{string}                        { return STRING; }
{id}                            { return ID; }
{to_ignore}                     {}
.                               { errorLex(yylineno); }

%%

