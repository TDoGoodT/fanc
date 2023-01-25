//
// Created by Snir Bachar on 20/12/2022.
//
#include "types.hpp"
#include "hw3_output.hpp"
#include "validations.hpp"
#include <vector>
#include <string>

using namespace std;
using namespace output;

static _T_Type::Type get_type(_T_Exp *pExp, _T_Exp *pExp1);

vector<string> _T_Formals::to_strings() const {
    vector<string> strings;
    for (auto formal: formal_list->formal_list) {
        strings.push_back(formal->id->type->to_string());
    }
    return strings;
}

string _T_FuncDecl::get_name() const {
    auto formal_names = formals->to_strings();
    return makeFunctionType(id->type->to_string(), formal_names);
}

string _T_RetType::to_string() {
    return type->to_string();
}

std::map<_T_Type::Type, std::string> type_to_name{
        {_T_Type::Type::_INT_,    "INT"},
        {_T_Type::Type::_BOOL_,   "BOOL"},
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

static _T_Type::Type get_type(_T_Exp *pExp, _T_Exp *pExp1) {
    if (pExp->type->typeCase == _T_Type::_INT_ || pExp1->type->typeCase == _T_Type::_INT_) {
        return _T_Type::_INT_;
    }
    if (pExp->type->typeCase == _T_Type::_BYTE_ || pExp1->type->typeCase == _T_Type::_BYTE_) {
        return _T_Type::_BYTE_;
    }
    output::errorMismatch(yylineno);
    exit(1);
}

bool _T_Binop::is_legal(int line) const {
    if (!is_numeric(r_exp->type->typeCase) || !is_numeric(r_exp->type->typeCase)) {
        errorMismatch(line);
        return false;
    }
    return true;
}
