//
// Created by Snir Bachar on 20/12/2022.
//
#include "types.hpp"
#include "hw3_output.hpp"
#include "visitor.hpp"
#include <vector>
#include <string>

using namespace std;
using namespace output;


vector<string> Formals::to_strings() const {
    vector<string> strings;
    for (auto formal: formal_list->formalList) {
        strings.push_back(formal->id->type->to_string());
    }
    return strings;
}

void Formals::accept() {
    Visitor::visit(this);
}

string FuncDecl::get_name() const {
    auto formal_names = formals->to_strings();
    return makeFunctionType(id->type->to_string(), formal_names);
}

void FuncDecl::accept() {
    Visitor::visit(this);
}

string RetType::to_string() const {
    return type->to_string();
}

void RetType::accept() {
    Visitor::visit(this);
}

std::map<Type::TypeCase, std::string> type_to_name{
        {Type::TypeCase::INT,    "i32"},
        {Type::TypeCase::BOOL,   "i1"},
        {Type::TypeCase::STRING, "STRING"},
        {Type::TypeCase::VOID,   "VOID"},
        {Type::TypeCase::BYTE, "i8"}
};

string Type::to_string() const {
    return type_to_name.at(typeCase);
}

bool is_numeric(Type::TypeCase type) {
    return type == Type::TypeCase::INT || type == Type::TypeCase::BYTE;
}

bool Relop::is_legal(int line) const {
    if (!is_numeric(rExp->type->typeCase) || !is_numeric(rExp->type->typeCase)) {
        errorMismatch(line);
        return false;
    }
    return true;
}

void Relop::accept() {
    Visitor::visit(this);
}

static string to_string(RelopCase relopCase) {
    switch (relopCase) {
        case GT_:
            return ">";
            break;
        case LT_:
            return "<";
            break;
        case GE_:
            return ">=";
            break;
        case LE_:
            return "<=";
            break;
        case EQ_:
            return "==";
            break;
        case NE_:
            return "!=";
            break;
    }
    return "";
}


bool Binop::is_legal(int line) const {
    if (!is_numeric(rExp->type->typeCase) || !is_numeric(rExp->type->typeCase)) {
        errorMismatch(line);
        return false;
    }
    return true;
}

void Binop::accept() {
    Visitor::visit(this);
}

void Continue::accept() {
    Visitor::visit(this);
}

void Break::accept() {
    Visitor::visit(this);
}

void Program::accept() {
    Visitor::visit(this);
}

void Funcs::accept() {
    Visitor::visit(this);
}

void FormalsList::accept() {
    Visitor::visit(this);
}

void FormalDecl::accept() {
    Visitor::visit(this);
}

void Statements::accept() {
    Visitor::visit(this);
}

void While::accept() {
    Visitor::visit(this);
}

void If_pattern::accept() {
    Visitor::visit(this);
}

void Return::accept() {
    Visitor::visit(this);
}

void FunctionCall::accept() {
    Visitor::visit(this);
}

void Call::accept() {
    Visitor::visit(this);
}

void Int::accept() {
    Visitor::visit(this);
}

void Number::accept() {
    Visitor::visit(this);
}

void CallExp::accept() {
    Visitor::visit(this);
}
void Id::accept() {
    Visitor::visit(this);
}


void Exp::accept() {
    Visitor::visit(this);
}

Type::TypeCase Exp::get_type(Exp *pExp, Exp *pExp1) {
    if (pExp->type->typeCase == Type::INT || pExp1->type->typeCase == Type::INT) {
        return Type::INT;
    }
    if (pExp->type->typeCase == Type::BYTE || pExp1->type->typeCase == Type::BYTE) {
        return Type::BYTE;
    }
    output::errorMismatch(yylineno);
    exit(1);
}

void Byte::accept() {
    Visitor::visit(this);
}

void Cast::accept() {
    Visitor::visit(this);
}

void LogicOp::accept() {
    Visitor::visit(this);
}

void And::accept() {
    Visitor::visit(this);
}

void Or::accept() {
    Visitor::visit(this);
}

void Not::accept() {
    Visitor::visit(this);
}

void String::accept() {
    Visitor::visit(this);
}

void Trinari::accept() {
    Visitor::visit(this);
}

void Bool::accept() {
    Visitor::visit(this);
}

void Statement::accept() {
    Visitor::visit(this);
}

void Declaration::accept() {
    Visitor::visit(this);
}

void Assignment::accept() {
    Visitor::visit(this);
}

void LateAssignment::accept() {
    Visitor::visit(this);
}
