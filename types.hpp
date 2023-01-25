//
// Created by snir on 12/17/22.
//

#ifndef ANALYZER_TYPES_HPP
#define ANALYZER_TYPES_HPP

#include <vector>
#include <string>
#include <map>
#include "hw3_output.hpp"
#include "visitor.hpp"

using namespace std;


struct _T_ {
    _T_() {}
    virtual void accept() const = 0;
    string &place = *new string();
};
struct _Non_T_ : public _T_  {
    _Non_T_() {}
};

struct _T_Type {
    enum Type {
        _INT_, _BOOL_, _STRING_, _VOID_, _BYTE_
    } typeCase;

    std::string to_string() const;

    _T_Type(Type type) : typeCase(type) {}

};

struct _T_Exp : public _Non_T_ {
    void accept() const override;

    struct _T_Type *type;
    int *value;

    _T_Exp(struct _T_Type *type) : type(type) {
        accept();
    }

    _T_Exp(struct _T_Type *type, int *value) : type(type), value(value) {
        accept();
    }

    static _T_Type::Type get_type(_T_Exp *pExp, _T_Exp *pExp1);
};



struct _T_ExpList : public _Non_T_ {
    void accept() const override {}

    std::vector<struct _T_Exp *> exp_list;

    _T_ExpList() {}

    _T_ExpList(struct _T_Exp *exp) { exp_list.push_back(exp); }
};

struct _T_Id : public _T_Exp {
    void accept() const override {}

    std::string id;

    _T_Id(string id, struct _T_Type *type) : id(id), _T_Exp(type) {}
};

struct _T_CallExp : public _T_Exp {
    void accept() const override;

    struct _T_Id *function;
    struct _T_ExpList *args;

    _T_CallExp(struct _T_Id *function) : _T_Exp(function->type), function(function) {accept();}

    _T_CallExp(struct _T_Id *function, struct _T_ExpList *args) : _T_Exp(function->type), function(function),
                                                                  args(args) {accept();}
};

struct _T_Number : public _T_Exp {
    void accept() const override;

    _T_Number(int *value, struct _T_Type *type) : _T_Exp(type, value) {accept();}
};

struct _T_Int : public _T_Number {
    void accept() const override;

    _T_Int(int *value) : _T_Number(value, new _T_Type(_T_Type::_INT_)) {accept();}
};

struct _T_Byte : public _T_Number {
    void accept() const override;

    _T_Byte(int *value) : _T_Number(value, new _T_Type(_T_Type::_BYTE_)) {accept();}
};

struct _T_Cast : public _T_Exp {
    void accept() const override;

    struct _T_Type *toType;
    struct _T_Exp *exp;

    _T_Cast(struct _T_Type *toType, struct _T_Exp *exp) : toType(toType), exp(exp), _T_Exp(toType) {accept();}
};

struct _T_LogicOp : public _T_Exp {
    void accept() const override;

    struct _T_Exp *r_value;
    struct _T_Exp *l_value;

    _T_LogicOp(struct _T_Exp *r_value, struct _T_Exp *l_value) : r_value(r_value), l_value(l_value),
                                                                 _T_Exp(new _T_Type(_T_Type::_BOOL_)) {accept();}
};

struct _T_And : public _T_LogicOp {
    void accept() const override;

    _T_And(struct _T_Exp *r_value, struct _T_Exp *l_value) : _T_LogicOp(r_value, l_value) {accept();}
};

struct _T_Or : public _T_LogicOp {
    void accept() const override;

    _T_Or(struct _T_Exp *r_value, struct _T_Exp *l_value) : _T_LogicOp(r_value, l_value) {accept();}
};

struct _T_Not : public _T_Exp {
    void accept() const override;

    struct _T_Exp *value;

    _T_Not(struct _T_Exp *value) : value(value), _T_Exp(new _T_Type(_T_Type::_BOOL_)) {accept();}
};

struct _T_Binop : public _T_Exp {
    void accept() const override;

    enum BinopCase {
        _PLUS_, _MINUS_, _MULT_, _DIV_
    } binopCase;
    struct _T_Exp *r_exp;
    struct _T_Exp *l_exp;

    _T_Binop(struct _T_Exp *l_exp, struct _T_Exp *r_exp, BinopCase binopCase) : r_exp(r_exp), l_exp(l_exp),
                                                                                binopCase(binopCase), _T_Exp(new _T_Type(
                    get_type(r_exp, l_exp))) {accept();};

    _T_Binop(struct _T_Exp *l_exp, struct _T_Exp *r_exp, struct _T_Type *type) : r_exp(r_exp), l_exp(l_exp),
                                                                                 _T_Exp(type) {accept();}

    bool is_legal(int line) const;
};

struct _T_Relop : public _T_Exp {
    void accept() const override;

    struct _T_Exp *r_exp;
    struct _T_Exp *l_exp;

    _T_Relop(struct _T_Exp *r_exp, struct _T_Exp *l_exp) : r_exp(r_exp), l_exp(l_exp),
                                                           _T_Exp(new _T_Type(_T_Type::_BOOL_)) {accept();}

    bool is_legal(int line) const;
};

struct _T_String : public _T_Exp {
    void accept() const override;

    std::string value;

    explicit _T_String(string value) : _T_Exp(new _T_Type(_T_Type::_STRING_)), value(value) {accept();}
};

struct _T_Trinari : public _T_Exp {
    void accept() const override;

    struct _T_Exp *true_exp;
    struct _T_Exp *cond_exp;
    struct _T_Exp *false_exp;

    _T_Trinari(struct _T_Exp *true_exp, struct _T_Exp *cond_exp, struct _T_Exp *false_exp) : true_exp(true_exp),
                                                                                             cond_exp(cond_exp),
                                                                                             false_exp(false_exp),
                                                                                             _T_Exp(true_exp->type) {accept();}
};

struct _T_Bool : public _T_Exp {
    void accept() const override;

    // rule 2
    bool value;

    explicit _T_Bool(bool value) : _T_Exp(new _T_Type(_T_Type::_BOOL_)), value(value) {accept();}
};

struct _T_Statement : public _Non_T_ {
    void accept() const override;

    _T_Statement() = default;
};

struct _T_Declaration : public _T_Statement {
    void accept() const override;

    struct _T_Type *type;
    struct _T_Id *id;

    _T_Declaration(struct _T_Type *type, struct _T_Id *id) : type(type), id(id) {accept();}
};

struct _T_Assignment : public _T_Statement {
    void accept() const override;

    struct _T_Type *type;
    struct _T_Id *id;
    struct _T_Exp *value;

    _T_Assignment(struct _T_Type *type, struct _T_Id *id, struct _T_Exp *value) : type(type), id(id), value(value) {accept();}
};

struct _T_LateAssignment : public _T_Statement {
    void accept() const override;

    struct _T_Id *id;
    struct _T_Exp *value;

    _T_LateAssignment(struct _T_Id *id, struct _T_Exp *value) : id(id), value(value) {accept();}
};

struct _T_Call : public _T_Statement {
    void accept() const override;

    struct _T_CallExp *callExp;

    _T_Call(struct _T_CallExp *callExp) : callExp(callExp) {accept();}
};

struct _T_FunctionCall : public _T_Statement {
    void accept() const override;

    struct _T_Id *function_id;
    struct _T_ExpList *args;

    _T_FunctionCall(struct _T_Call *call) : function_id(call->callExp->function), args(call->callExp->args) {accept();}

    _T_FunctionCall(struct _T_Id *function_id, struct _T_ExpList *args) : function_id(function_id), args(args) {accept();}
};

struct _T_Return : public _T_Statement {
    void accept() const override;

    struct _T_Exp *value;

    _T_Return() : value(nullptr) {accept();}

    _T_Return(struct _T_Exp *value) : value(value) {accept();}
};

struct _T_If_pattern : public _T_Statement {
    void accept() const override;

    struct _T_Exp *condition;
    struct _T_Statement *true_stmnt;
    struct _T_Statement *false_stmnt;

    _T_If_pattern(struct _T_Exp *condition, struct _T_Statement *stmnt) : condition(condition), true_stmnt(stmnt) {accept();}

    _T_If_pattern(struct _T_Exp *condition, struct _T_Statement *true_stmnt, struct _T_Statement *false_stmnt)
            : condition(condition), true_stmnt(true_stmnt), false_stmnt(false_stmnt) {accept();}
};

struct _T_While : public _T_Statement {
    void accept() const override;

    struct _T_Exp *condition;
    struct _T_Statement *stmnt;

    _T_While(struct _T_Exp *condition, struct _T_Statement *stmnt) : condition(condition), stmnt(stmnt) {accept();}
};

struct _T_Statements : public _T_Statement {
    void accept() const override;

    std::vector<struct _T_Statement *> statement_list;

    _T_Statements() = default;

    _T_Statements(struct _T_Statement *stmnt1) {
        statement_list.push_back(stmnt1);
        accept();
    }

    _T_Statements(struct _T_Statement *stmnt1, struct _T_Statement *stmnt2) {
        statement_list.push_back(stmnt1);
        statement_list.push_back(stmnt2);
        accept();
    }

};

struct _T_RetType : public _Non_T_ {
    void accept() const override;

    struct _T_Type *type;

    _T_RetType(_T_Type::Type type) : type(new _T_Type(type)) {accept();}

    _T_RetType(struct _T_Type *type) : type(type) {accept();}

    string to_string();
};

struct _T_FormalDecl : public _Non_T_ {
    void accept() const override;

    struct _T_Id *id;

    _T_FormalDecl(struct _T_Id *id) : id(id) {accept();}
};

struct _T_FormalsList : public _Non_T_ {
    void accept() const override;

    std::vector<struct _T_FormalDecl *> formal_list;

    _T_FormalsList() {accept();}

    _T_FormalsList(struct _T_FormalDecl *formal) : formal_list({formal}) {accept();}
};

struct _T_Formals : public _Non_T_ {
    void accept() const override;

    struct _T_FormalsList *formal_list;

    _T_Formals() : formal_list(new _T_FormalsList()) {accept();}

    _T_Formals(struct _T_FormalsList *formal_list) : formal_list(formal_list) {accept();}

    vector<string> to_strings() const;
};

struct _T_FuncDecl : public _Non_T_ {
    void accept() const override;

    struct _T_Id *id;
    struct _T_Formals *formals;
    struct _T_Statements *stmts;

    _T_FuncDecl(struct _T_Id *id, struct _T_Formals *formals) : id(id), formals(formals) {accept();}

    _T_FuncDecl(struct _T_Id *id, struct _T_Formals *formals, struct _T_Statements *stmts) : id(id), formals(formals),
                                                                                             stmts(stmts) {accept();}

    string get_name() const;
};

struct _T_Funcs : public _Non_T_ {
    void accept() const override;

    std::vector<struct _T_FuncDecl *> func_list;

    _T_Funcs() {accept();}
};

struct _T_Program : public _Non_T_ {
    void accept() const override;

    struct _T_Funcs *funcs;

    _T_Program(struct _T_Funcs *funcs) : funcs(funcs) {accept();}
};


struct _T_Break : public _T_Statement {
    void accept() const override;

    _T_Break() {accept();}
};

struct _T_Continue : public _T_Statement {
    void accept() const override;

    _T_Continue() {accept();}
};


#endif //ANALYZER_TYPES_HPP
