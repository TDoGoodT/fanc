#ifndef _236360_3_
#define _236360_3_
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <iostream>
#include "types.hpp"
#include "analyzer.tab.hpp"
#include <sstream>
#include <map>
using namespace std;

namespace output {
    void endScope();
    void printID(const string& id, int offset, const string& type);
    /* Do not save the string returned from this function in a data structure as it is not dynamically allocated and will be destroyed(!) at the end of the calling scope. */
    string makeFunctionType(const string& retType, vector<string>& argTypes);
    void errorLex(int lineno);
    void errorSyn(int lineno);
    void errorUndef(int lineno, const string& id);
    void errorDef(int lineno, const string& id);
    void errorUndefFunc(int lineno, const string& id);
    void errorMismatch(int lineno);
    void errorPrototypeMismatch(int lineno, const string& id, vector<string>& argTypes);
    void errorUnexpectedBreak(int lineno);
    void errorUnexpectedContinue(int lineno);
    void errorMainMissing();
    void errorByteTooLarge(int lineno, const string& value);
    void printProductionRule(int);
    void applyProductionRule(int);
    void printToken(int);
}
const std::map<int, std::string> tokens{
        { ID, "ID"},
        { STRING, "STRING"},
        { NUM, "NUM"},
        { VOID, "VOID"},
        { INT, "INT"},
        { BYTE, "BYTE"},
        { TRUE, "TRUE"},
        { FALSE, "FALSE"},
        { RETURN, "RETURN"},
        { IF, "IF"},
        { ELSE, "ELSE"},
        { WHILE, "WHILE"},
        { BREAK, "BREAK"},
        { CONTINUE, "CONTINUE"},
        { SC, "SC"},
        { COMMA, "COMMA"},
        { BOOL, "BOOL"},
        { ADD, "ADD"},
        { SUB, "SUB"},
        { RELOP, "RELOP"},
        { AND, "AND"},
        { RPAREN, "RPAREN"},
        { RBRACE, "RBRACE"},
        { B, "B"},
        { ASSIGN, "ASSIGN"},
        { LPAREN, "LPAREN"},
        { LBRACE, "LBRACE"},
        { NOT, "NOT"},
        { OR, "OR"},
        { DIV, "DIV"},
        { MULT, "MULT"},
};
const std::string rules[] = {
        "Program -> Funcs",
        "Funcs -> epsilon",
        "Funcs -> FuncDecl Funcs",
        "FuncDecl -> RetType ID LPAREN Formals RPAREN LBRACE Statements RBRACE",
        "RetType -> Type",
        "RetType ->  VOID",
        "Formals -> epsilon",
        "Formals -> FormalsList",
        "FormalsList -> FormalDecl",
        "FormalsList -> FormalDecl COMMA FormalsList",
        "FormalDecl -> Type ID",
        "Statements -> Statement",
        "Statements -> Statements Statement",
        "Statement -> LBRACE Statements RBRACE",
        "Statement -> Type ID SC",
        "Statement -> Type ID ASSIGN Exp SC",
        "Statement -> ID ASSIGN Exp SC",
        "Statement -> Call SC",
        "Statement -> RETURN SC",
        "Statement -> RETURN Exp SC",
        "Statement -> IF LPAREN Exp RPAREN Statement",
        "Statement -> IF LPAREN Exp RPAREN Statement ELSE Statement",
        "Statement -> WHILE LPAREN Exp RPAREN Statement",
        "Statement -> BREAK SC",
        "Statement -> CONTINUE SC",
        "Call -> ID LPAREN ExpList RPAREN",
        "Call -> ID LPAREN RPAREN",
        "ExpList -> Exp",
        "ExpList -> Exp COMMA ExpList",
        "Type -> INT",
        "Type -> BYTE",
        "Type -> BOOL",
        "Exp -> LPAREN Exp RPAREN",
        "Exp -> Exp IF LPAREN Exp RPAREN ELSE LPAREN Exp RPAREN",
        "Exp -> Exp BINOP Exp",
        "Exp -> ID",
        "Exp -> Call",
        "Exp -> NUM",
        "Exp -> NUM B",
        "Exp -> STRING",
        "Exp -> TRUE",
        "Exp -> FALSE",
        "Exp -> NOT Exp",
        "Exp -> Exp AND Exp",
        "Exp -> Exp OR Exp",
        "Exp -> Exp RELOP Exp",
        "Exp -> LPAREN Type RPAREN Exp"
};

extern int yylineno;
extern char* yytext;
extern int yyleng;
extern int yydebug;
extern int yylex();
#endif
