//
// Created by snir on 12/17/22.
//

#ifndef ANALYZER_TYPES_HPP
#define ANALYZER_TYPES_HPP

#include <vector>
#include <string>
#include <map>
#include "bp.hpp"

using namespace std;


struct _T_ {
    _T_() {}

    virtual void accept() = 0;

    string &place = *new string();
};

struct _Non_T_ : public _T_ {
    _Non_T_() {}
};

struct _T_Type {
    enum Type {
        _INT_, _BOOL_, _STRING_, _VOID_, _BYTE_
    } typeCase;

    string to_string();

    _T_Type(Type type) : typeCase(type) {}

};

struct _T_Exp : public _Non_T_ {
    void accept() override;

    bool is_bool = false;

    struct _T_Type *type;
    int *value = nullptr;
    vector<pair<int, BranchLabelIndex>> nextList;
    vector<pair<int, BranchLabelIndex>> breakList;
    vector<pair<int, BranchLabelIndex>> continueList;
    vector<pair<int, BranchLabelIndex>> trueList;
    vector<pair<int, BranchLabelIndex>> falseList;

    _T_Exp(struct _T_Type *type) : type(type) {

    }

    _T_Exp(struct _T_Type *type, int *value) : type(type), value(value) {

    }

    _T_Exp(struct _T_Type *type, int *value, bool in_statement) : type(type), value(value), is_bool(in_statement) {

    }

    static _T_Type::Type get_type(_T_Exp *pExp, _T_Exp *pExp1);
};



struct _T_ExpList : public _Non_T_ {
    void accept() override {}

    vector<struct _T_Exp *> exp_list;

    _T_ExpList() {}

    _T_ExpList(struct _T_Exp *exp) { exp_list.push_back(exp); }
};



struct _T_Id : public _T_Exp {
    void accept() override {}

    string id;

    _T_Id(string id, struct _T_Type *type) : id(id), _T_Exp(type) {}
};

struct _T_CallExp : public _T_Exp {
    void accept() override;

    struct _T_Id *function;
    struct _T_ExpList *args;

    _T_CallExp(struct _T_Id *function) : _T_Exp(function->type), function(function) {}

    _T_CallExp(struct _T_Id *function, struct _T_ExpList *args) : _T_Exp(function->type), function(function),
                                                                  args(args) {}
};

struct _T_Number : public _T_Exp {
    void accept() override;

    _T_Number(int *value, struct _T_Type *type) : _T_Exp(type, value) {}
};

struct _T_Int : public _T_Number {
    void accept() override;

    _T_Int(int *value) : _T_Number(value, new _T_Type(_T_Type::_INT_)) {}
};

struct _T_Byte : public _T_Number {
    void accept() override;

    _T_Byte(int *value) : _T_Number(value, new _T_Type(_T_Type::_BYTE_)) {}
};

struct _T_Cast : public _T_Exp {
    void accept() override;

    struct _T_Type *toType;
    struct _T_Exp *exp;

    _T_Cast(struct _T_Type *toType, struct _T_Exp *exp) : toType(toType), exp(exp), _T_Exp(toType) {}
};

struct _T_LogicOp : public _T_Exp {
    void accept() override;

    struct _T_Exp *r_value;
    struct _T_Exp *l_value;

    _T_LogicOp(struct _T_Exp *r_value, struct _T_Exp *l_value) : r_value(r_value), l_value(l_value),
                                                                 _T_Exp(new _T_Type(_T_Type::_BOOL_)) {}
};

struct _T_And : public _T_LogicOp {
    void accept() override;

    _T_And(struct _T_Exp *r_value, struct _T_Exp *l_value) : _T_LogicOp(r_value, l_value) {}
};

struct _T_Or : public _T_LogicOp {
    void accept() override;

    _T_Or(struct _T_Exp *r_value, struct _T_Exp *l_value) : _T_LogicOp(r_value, l_value) {}
};

struct _T_Not : public _T_Exp {
    void accept() override;

    struct _T_Exp *value;

    _T_Not(struct _T_Exp *value) : value(value), _T_Exp(new _T_Type(_T_Type::_BOOL_)) {}
};

struct _T_Binop : public _T_Exp {
    void accept() override;

    enum BinopCase {
        _PLUS_, _MINUS_, _MULT_, _DIV_
    } binopCase;
    struct _T_Exp *r_exp;
    struct _T_Exp *l_exp;

    _T_Binop(struct _T_Exp *l_exp, struct _T_Exp *r_exp, BinopCase binopCase) : r_exp(r_exp), l_exp(l_exp),
                                                                                binopCase(binopCase),
                                                                                _T_Exp(new _T_Type(
                                                                                        get_type(r_exp,
                                                                                                 l_exp))) {};

    _T_Binop(struct _T_Exp *l_exp, struct _T_Exp *r_exp, struct _T_Type *type) : r_exp(r_exp), l_exp(l_exp),
                                                                                 _T_Exp(type) {}

    bool is_legal(int line);
};

enum RelopCase {
    _GT_, _GE_, _LT_, _LE_, _EQ_, _NE_
};

struct _T_Relop : public _T_Exp {
    void accept() override;

    struct _T_Exp *rExp;
    struct _T_Exp *lExp;
    RelopCase relopCase;

    _T_Relop(struct _T_Exp *l_exp, struct _T_Exp *r_exp, RelopCase relopCase) : rExp(r_exp), lExp(l_exp),
                                                                                _T_Exp(new _T_Type(_T_Type::_BOOL_)),
                                                                                relopCase(relopCase) {}

    bool is_legal(int line);
};


struct _T_String : public _T_Exp {
    void accept() override;

    string value;

    explicit _T_String(string value) : _T_Exp(new _T_Type(_T_Type::_STRING_)), value(value) {}
};

struct _T_Trinari : public _T_Exp {
    void accept() override;

    struct _T_Exp *true_exp;
    struct _T_Exp *cond_exp;
    struct _T_Exp *false_exp;

    _T_Trinari(struct _T_Exp *true_exp, struct _T_Exp *cond_exp, struct _T_Exp *false_exp) : true_exp(true_exp),
                                                                                             cond_exp(cond_exp),
                                                                                             false_exp(false_exp),
                                                                                             _T_Exp(true_exp->type) {}
};

struct _T_Bool : public _T_Exp {
    void accept() override;

    // rule 2
    bool value;

    explicit _T_Bool(bool value) : _T_Exp(new _T_Type(_T_Type::_BOOL_)), value(value) {}
};

struct _T_Statement : public _Non_T_ {
    void accept() override;

    _T_Statement() = default;
};



struct _T_Declaration : public _T_Statement {
    void accept() override;

    struct _T_Type *type;
    struct _T_Id *id;

    _T_Declaration(struct _T_Type *type, struct _T_Id *id) : type(type), id(id) {}
};

struct _T_Assignment : public _T_Statement {
    void accept() override;

    struct _T_Type *type;
    struct _T_Id *id;
    struct _T_Exp *value;

    _T_Assignment(struct _T_Type *type, struct _T_Id *id, struct _T_Exp *value) : type(type), id(id),
                                                                                  value(value) {}
};

struct _T_LateAssignment : public _T_Statement {
    void accept() override;

    struct _T_Id *id;
    struct _T_Exp *value;

    _T_LateAssignment(struct _T_Id *id, struct _T_Exp *value) : id(id), value(value) {}
};

struct _T_Call : public _T_Statement {
    void accept() override;

    struct _T_CallExp *callExp;

    _T_Call(struct _T_CallExp *callExp) : callExp(callExp) {}
};

struct _T_FunctionCall : public _T_Statement {
    void accept() override;

    struct _T_Id *function_id;
    struct _T_ExpList *args;

    _T_FunctionCall(struct _T_Call *call) : function_id(call->callExp->function),
                                            args(call->callExp->args) {}

    _T_FunctionCall(struct _T_Id *function_id, struct _T_ExpList *args) : function_id(function_id),
                                                                          args(args) {}
};

struct _T_Return : public _T_Statement {
    void accept() override;

    struct _T_Exp *value;

    _T_Return() : value(nullptr) {}

    _T_Return(struct _T_Exp *value) : value(value) {}
};


struct _T_If_pattern : public _T_Statement {
    void accept() override;

    struct _T_Exp *condition;
    struct _T_Statement *true_stmnt;
    struct _T_Statement *false_stmnt;

    _T_If_pattern(struct _T_Exp *condition, struct _T_Statement *stmnt) : condition(condition),
                                                                          true_stmnt(stmnt) {}

    _T_If_pattern(struct _T_Exp *condition, struct _T_Statement *true_stmnt, struct _T_Statement *false_stmnt)
            : condition(condition), true_stmnt(true_stmnt), false_stmnt(false_stmnt) {}
};

struct _T_While : public _T_Statement {
    void accept() override;

    struct _T_Exp *condition;
    struct _T_Statement *stmnt;

    _T_While(struct _T_Exp *condition, struct _T_Statement *stmnt) : condition(condition), stmnt(stmnt) {}
};
struct _T_Statements : public _T_Statement {
    void accept() override;

    vector<struct _T_Statement *> statement_list;

    _T_Statements() = default;

    _T_Statements(struct _T_Statement *stmnt1) {
        statement_list.push_back(stmnt1);

    }

    _T_Statements(struct _T_Statement *stmnt1, struct _T_Statement *stmnt2) {
        statement_list.push_back(stmnt1);
        statement_list.push_back(stmnt2);

    }

};


struct _T_RetType : public _Non_T_ {
    void accept() override;

    struct _T_Type *type;

    _T_RetType(_T_Type::Type type) : type(new _T_Type(type)) {}

    _T_RetType(struct _T_Type *type) : type(type) {}

    string to_string();
};





struct _T_FormalDecl : public _Non_T_ {
    void accept() override;

    struct _T_Id *id;

    _T_FormalDecl(struct _T_Id *id) : id(id) {}
};

struct _T_FormalsList : public _Non_T_ {
    void accept() override;

    vector<struct _T_FormalDecl *> formal_list;

    _T_FormalsList() {}

    _T_FormalsList(struct _T_FormalDecl *formal) : formal_list({formal}) {}
};


struct _T_Formals : public _Non_T_ {
    void accept() override;

    struct _T_FormalsList *formal_list;

    _T_Formals() : formal_list(new _T_FormalsList()) {}

    _T_Formals(struct _T_FormalsList *formal_list) : formal_list(formal_list) {}

    vector<string> to_strings();
};




struct _T_FuncDecl : public _Non_T_ {
    void accept() override;

    struct _T_Id *id;
    struct _T_Formals *formals;
    struct _T_Statements *stmts;

    _T_FuncDecl(struct _T_Id *id, struct _T_Formals *formals) : id(id), formals(formals) {}

    _T_FuncDecl(struct _T_Id *id, struct _T_Formals *formals, struct _T_Statements *stmts) : id(id), formals(formals),
                                                                                             stmts(stmts) {}

    string get_name();
};



struct _T_Funcs : public _Non_T_ {
    void accept() override;

    vector<struct _T_FuncDecl *> func_list;

    _T_Funcs() {}
};



struct _T_Program : public _Non_T_ {
    _T_Program(struct _T_Funcs *funcs) : funcs(funcs) {}

    void accept() override;

    struct _T_Funcs *funcs;

};




struct _T_Break : public _T_Statement {
    void accept() override;

    _T_Break() {}
};


struct _T_Continue : public _T_Statement {
    void accept() override;

    _T_Continue() {}
};


#endif //ANALYZER_TYPES_HPP
