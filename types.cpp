//
// Created by Snir Bachar on 20/12/2022.
//
#include "types.hpp"
#include "hw3_output.hpp"
#include <vector>
#include <string>

using namespace std;
using namespace output;

Visitor& visitor = Visitor::instance();

vector<string> _T_Formals::to_strings() const {
    vector<string> strings;
    for (auto formal: formal_list->formal_list) {
        strings.push_back(formal->id->type->to_string());
    }
    return strings;
}

void _T_Formals::accept() const {
    visitor.visit(this);
}

string _T_FuncDecl::get_name() const {
    auto formal_names = formals->to_strings();
    return makeFunctionType(id->type->to_string(), formal_names);
}

void _T_FuncDecl::accept() const {
    visitor.visit(this);
}

string _T_RetType::to_string() {
    return type->to_string();
}

void _T_RetType::accept() const {
    visitor.visit(this);
}

std::map<_T_Type::Type, std::string> type_to_name{
        {_T_Type::Type::_INT_,    "i32"},
        {_T_Type::Type::_BOOL_,   "i1"},
        {_T_Type::Type::_STRING_, "STRING"},
        {_T_Type::Type::_VOID_,   "VOID"},
        {_T_Type::Type::_BYTE_,   "BYTE"}
};

string _T_Type::to_string() const {
    return type_to_name.at(typeCase);
}

bool is_numeric(_T_Type::Type type) {
    return type == _T_Type::Type::_INT_ || type == _T_Type::Type::_BYTE_;
}

bool _T_Relop::is_legal(int line) const {
    if (!is_numeric(r_exp->type->typeCase) || !is_numeric(r_exp->type->typeCase)) {
        errorMismatch(line);
        return false;
    }
    return true;
}

void _T_Relop::accept() const {
    visitor.visit(this);
}

bool _T_Binop::is_legal(int line) const {
    if (!is_numeric(r_exp->type->typeCase) || !is_numeric(r_exp->type->typeCase)) {
        errorMismatch(line);
        return false;
    }
    return true;
}

void _T_Binop::accept() const {
    visitor.visit(this);
}

void _T_Continue::accept() const {
    visitor.visit(this);
}

void _T_Break::accept() const {
    visitor.visit(this);
}

void _T_Program::accept() const {
    visitor.visit(this);
}

void _T_Funcs::accept() const {
    visitor.visit(this);
}

void _T_FormalsList::accept() const {
    visitor.visit(this);
}

void _T_FormalDecl::accept() const {
    visitor.visit(this);
}

void _T_Statements::accept() const {
    visitor.visit(this);
}

void _T_While::accept() const {
    visitor.visit(this);
}

void _T_If_pattern::accept() const {
    visitor.visit(this);
}

void _T_Return::accept() const {
    visitor.visit(this);
}

void _T_FunctionCall::accept() const {
    visitor.visit(this);
}

void _T_Call::accept() const {
    visitor.visit(this);
}

void _T_Int::accept() const {
    visitor.visit(this);
}

void _T_Number::accept() const {
    visitor.visit(this);
}

void _T_CallExp::accept() const {
    visitor.visit(this);
}


void _T_Exp::accept() const {
    visitor.visit(this);
}

_T_Type::Type _T_Exp::get_type(_T_Exp *pExp, _T_Exp *pExp1) {
    if (pExp->type->typeCase == _T_Type::_INT_ || pExp1->type->typeCase == _T_Type::_INT_) {
        return _T_Type::_INT_;
    }
    if (pExp->type->typeCase == _T_Type::_BYTE_ || pExp1->type->typeCase == _T_Type::_BYTE_) {
        return _T_Type::_BYTE_;
    }
    output::errorMismatch(yylineno);
    exit(1);
}

void _T_Byte::accept() const {
    visitor.visit(this);
}

void _T_Cast::accept() const {
    visitor.visit(this);
}

void _T_LogicOp::accept() const {
    visitor.visit(this);
}

void _T_And::accept() const {
    visitor.visit(this);
}

void _T_Or::accept() const {
    visitor.visit(this);
}

void _T_Not::accept() const {
    visitor.visit(this);
}

void _T_String::accept() const {
    visitor.visit(this);
}

void _T_Trinari::accept() const {
    visitor.visit(this);
}

void _T_Bool::accept() const {
    visitor.visit(this);
}

void _T_Statement::accept() const {
    visitor.visit(this);
}

void _T_Declaration::accept() const {
    visitor.visit(this);
}

void _T_Assignment::accept() const {
    visitor.visit(this);
}

void _T_LateAssignment::accept() const {
    visitor.visit(this);
}
