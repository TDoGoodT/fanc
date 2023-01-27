//
// Created by snir on 12/17/22.
//

#ifndef ANALYZER_TYPES_HPP
#define ANALYZER_TYPES_HPP

#include <vector>
#include <string>
#include <map>
#include "bp.hpp"
#include "markers.hpp"

using namespace std;


struct T  {
    T() {}

    virtual void accept() = 0;

    string &place = *new string();
};

struct NonT : public T {
    NonT() {}
};

struct Type {
    enum TypeCase {
        INT, BOOL, STRING, VOID, BYTE
    } typeCase;

    string to_string() const;

    Type(TypeCase type) : typeCase(type) {}

};

struct Exp : public NonT {
    void accept() override;

    bool is_bool = false;

    struct Type *type;
    int *value = nullptr;
    vector<pair<int, BranchLabelIndex>> nextList;
    vector<pair<int, BranchLabelIndex>> breakList;
    vector<pair<int, BranchLabelIndex>> continueList;
    vector<pair<int, BranchLabelIndex>> trueList;
    vector<pair<int, BranchLabelIndex>> falseList;

    Exp(struct Type *type) : type(type) {

    }

    Exp(struct Type *type, int *value) : type(type), value(value) {

    }

    Exp(struct Type *type, int *value, bool in_statement) : type(type), value(value), is_bool(in_statement) {

    }

    static Type::TypeCase get_type(Exp *pExp, Exp *pExp1);
};



struct ExpList : public NonT {
    void accept() override {}

    vector<struct Exp *> exp_list;

    ExpList() {}

    ExpList(struct Exp *exp) { exp_list.push_back(exp); }
};



struct Id : public Exp {
    void accept() override {}

    string id;

    Id(string id, struct Type *type) : id(id), Exp(type) {}
};

struct CallExp : public Exp {
    void accept() override;

    struct Id *function;
    struct ExpList *args;

    CallExp(struct Id *function) : Exp(function->type), function(function) {}

    CallExp(struct Id *function, struct ExpList *args) : Exp(function->type), function(function),
                                                                  args(args) {}
};

struct Number : public Exp {
    void accept() override;

    Number(int *value, struct Type *type) : Exp(type, value) {}
};

struct Int : public Number {
    void accept() override;

    Int(int *value) : Number(value, new Type(Type::INT)) {}
};

struct Byte : public Number {
    void accept() override;

    Byte(int *value) : Number(value, new Type(Type::BYTE)) {}
};

struct Cast : public Exp {
    void accept() override;

    struct Type *toType;
    struct Exp *exp;

    Cast(struct Type *toType, struct Exp *exp) : toType(toType), exp(exp), Exp(toType) {}
};

struct LogicOp : public Exp {
    void accept() override;

    struct Exp *rExp;
    struct Exp *lExp;

    LogicOp(struct Exp *r_value, struct Exp *l_value) : rExp(r_value), lExp(l_value),
                                                           Exp(new Type(Type::BOOL)) {}
};

struct And : public LogicOp {
    void accept() override;
    struct AndMarker* andMarker;
    And(struct Exp *r_value, struct Exp *l_value, struct AndMarker* andMarker) : LogicOp(r_value, l_value), andMarker(std::move(andMarker)) {}
};

struct Or : public LogicOp {
    void accept() override;
    struct OrMarker*   orMarker;
    Or(struct Exp *r_value, struct Exp *l_value, struct OrMarker* orMarker) : LogicOp(r_value, l_value), orMarker(std::move(orMarker)) {}
};

struct Not : public Exp {
    void accept() override;

    struct Exp *exp;

    Not(struct Exp *value) : exp(value), Exp(new Type(Type::BOOL)) {}
};
enum BinopCase {
    _PLUS_, _MINUS_, _MULT_, _DIV_
};
struct Binop : public Exp {
    void accept() override;

    enum BinopCase binopCase;
    struct Exp *rExp;
    struct Exp *lExp;

    Binop(struct Exp *l_exp, struct Exp *r_exp, BinopCase binopCase) : rExp(r_exp), lExp(l_exp),
                                                                          binopCase(binopCase),
                                                                          Exp(new Type(
                                                                                        get_type(r_exp,
                                                                                                 l_exp))) {};

    Binop(struct Exp *l_exp, struct Exp *r_exp, struct Type *type) : rExp(r_exp), lExp(l_exp),
                                                                           Exp(type) {}

    bool is_legal(int line) const;
};

enum RelopCase {
    GT_, GE_, LT_, LE_, EQ_, NE_
};

struct Relop : public Exp {
    void accept() override;

    struct Exp *rExp;
    struct Exp *lExp;
    RelopCase relopCase;

    Relop(struct Exp *l_exp, struct Exp *r_exp, RelopCase relopCase) : rExp(r_exp), lExp(l_exp),
                                                                          Exp(new Type(Type::BOOL)),
                                                                          relopCase(relopCase) {}

    bool is_legal(int line) const;
};


struct String : public Exp {
    void accept() override;

    string value;

    explicit String(string value) : Exp(new Type(Type::STRING)), value(value) {}
};

struct Trinari : public Exp {
    void accept() override;

    struct Exp *true_exp;
    struct Exp *cond_exp;
    struct Exp *false_exp;

    Trinari(struct Exp *true_exp, struct Exp *cond_exp, struct Exp *false_exp) : true_exp(true_exp),
                                                                                    cond_exp(cond_exp),
                                                                                    false_exp(false_exp),
                                                                                    Exp(true_exp->type) {}
};

struct Bool : public Exp {
    void accept() override;

    // rule 2
    bool value;

    explicit Bool(bool value) : Exp(new Type(Type::BOOL)), value(value) {}
};

struct Statement : public NonT {
    void accept() override;

    Statement() = default;
};



struct Declaration : public Statement {
    void accept() override;

    struct Type *type;
    struct Id *id;

    Declaration(struct Type *type, struct Id *id) : type(type), id(id) {}
};

struct Assignment : public Statement {
    void accept() override;

    struct Type *type;
    struct Id *id;
    struct Exp *value;

    Assignment(struct Type *type, struct Id *id, struct Exp *value) : type(type), id(id),
                                                                               value(value) {}
};

struct LateAssignment : public Statement {
    void accept() override;

    struct Id *id;
    struct Exp *value;

    LateAssignment(struct Id *id, struct Exp *value) : id(id), value(value) {}
};

struct Call : public Statement {
    void accept() override;

    struct CallExp *callExp;

    Call(struct CallExp *callExp) : callExp(callExp) {}
};

struct FunctionCall : public Statement {
    void accept() override;

    struct Id *function_id;
    struct ExpList *args;

    FunctionCall(struct Call *call) : function_id(call->callExp->function),
                                            args(call->callExp->args) {}

    FunctionCall(struct Id *function_id, struct ExpList *args) : function_id(function_id),
                                                                          args(args) {}
};

struct Return : public Statement {
    void accept() override;

    struct Exp *value;

    Return() : value(nullptr) {}

    Return(struct Exp *value) : value(value) {}
};


struct If_pattern : public Statement {
    void accept() override;

    struct Exp *condition;
    struct Statement *true_stmnt;
    struct Statement *false_stmnt;

    If_pattern(struct Exp *condition, struct Statement *stmnt) : condition(condition),
                                                                       true_stmnt(stmnt) {}

    If_pattern(struct Exp *condition, struct Statement *true_stmnt, struct Statement *false_stmnt)
            : condition(condition), true_stmnt(true_stmnt), false_stmnt(false_stmnt) {}
};

struct While : public Statement {
    void accept() override;

    struct Exp *condition;
    struct Statement *stmnt;

    While(struct Exp *condition, struct Statement *stmnt) : condition(condition), stmnt(stmnt) {}
};
struct Statements : public Statement {
    void accept() override;

    vector<struct Statement *> statement_list;

    Statements() = default;

    Statements(struct Statement *stmnt1) {
        statement_list.push_back(stmnt1);

    }

    Statements(struct Statement *stmnt1, struct Statement *stmnt2) {
        statement_list.push_back(stmnt1);
        statement_list.push_back(stmnt2);

    }

};


struct RetType : public NonT {
    void accept() override;

    struct Type *type;

    RetType(Type::TypeCase type) : type(new Type(type)) {}

    RetType(struct Type *type) : type(type) {}

    string to_string() const;
};





struct FormalDecl : public NonT {
    void accept() override;

    struct Id *id;

    FormalDecl(struct Id *id) : id(id) {}
};

struct FormalsList : public NonT {
    void accept() override;

    vector<struct FormalDecl *> formalList;

    FormalsList() {}

    FormalsList(struct FormalDecl *formal) : formalList({formal}) {}
};


struct Formals : public NonT {
    void accept() override;

    struct FormalsList *formal_list;

    Formals() : formal_list(new FormalsList()) {}

    Formals(struct FormalsList *formal_list) : formal_list(formal_list) {}

    vector<string> to_strings() const;
};




struct FuncDecl : public NonT {
    void accept() override;

    struct Id *id;
    struct Formals *formals;
    struct Statements *stmts;

    FuncDecl(struct Id *id, struct Formals *formals) : id(id), formals(formals) {}

    FuncDecl(struct Id *id, struct Formals *formals, struct Statements *stmts) : id(id), formals(formals),
                                                                                          stmts(stmts) {}

    string get_name() const;
};



struct Funcs : public NonT {
    void accept() override;

    vector<struct FuncDecl *> func_list;

    Funcs() {}
};



struct Program : public NonT {
    Program(struct Funcs *funcs) : funcs(funcs) {}

    void accept() override;

    struct Funcs *funcs;

};




struct Break : public Statement {
    void accept() override;

    Break() {}
};


struct Continue : public Statement {
    void accept() override;

    Continue() {}
};


#endif //ANALYZER_TYPES_HPP
