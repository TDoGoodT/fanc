#include <cassert>
#include "markers.hpp"
#include "store.hpp"
#include "actions.hpp"
#include "validations.hpp"

//
// Created by Snir Bachar on 27/01/2023.
//
Store store = Store::instance();
using namespace actions;
using namespace validations;
Type* MakeType(Type::TypeCase type) {
    return new Type(type);
}

Exp* MakeExp(struct Type *type) {
    Exp *p = new Exp(type);
    p->accept();
    return p;

}

Id* MakeFId(Id* id){
    if(is_func_declared(&store, id)) {
        errorDef(yylineno, id->id);
        exit(1);
    }
    declare_func(&store, id);
    return id;
}
Id* MakeId(Id* id){
    if(!is_id_declared(&store, id)) {
        errorUndef(yylineno, id->id);
        exit(1);
    }
    Id* retId = get_id(&store, id);
    return retId;
}

Exp* MakeExp(struct Type *type, int *value) {
    Exp *p = new Exp(type, value);
    p->accept();
    return p;

}

Exp* MakeExp(struct Type *type, int *value, bool in_statement) {
    Exp *p = new Exp(type, value, in_statement);
    p->accept();
    return p;

}

ExpList *MakeExpList(Exp* exp, ExpList* exp_list) {
    exp_list->exp_list.insert(exp_list->exp_list.begin(), exp);
    return exp_list;
}

ExpList *MakeExpList(Exp *exp) {
    ExpList *p = new ExpList(exp);
    p->accept();
    return p;
}

Id* MakeId(string id, struct Type *type) {
    auto p = new Id(id, type);
    return p;
}
Id* MakeId(string text) {
    auto id = store.get_id(text);
    if(id != nullptr) {
        return id;
    } else {
        auto id = store.get_func(text);
        if(id != nullptr) {
            return id;
        } else {
            return MakeId(string(text), nullptr);
        }
    }
}
CallExp* MakeCallExp(struct Id *function) {
    if(!is_func_declared(&store, function)) {;
        errorUndefFunc(yylineno, function->id);
        exit(1);
    }
    auto id = get_func(&store, function);
    auto p = new CallExp(id);
    p->accept();
    return p;
}

CallExp* MakeCallExp(struct Id *function, struct ExpList *args) {
    if(!is_func_declared(&store, function)) {
        errorUndefFunc(yylineno, function->id);
        exit(1);
    }
    auto id = get_func(&store, function);
    auto p = new CallExp(id, args);
    p->accept();
    return p;
}

Number* MakeNumber(int *value) {
    auto p = new Number(value, nullptr);
    p->accept();
    return p;
}

Int* MakeInt(int *value) {
    auto p = new Int(value);
    p->accept();
    return p;
}

Byte* MakeByte(int *value) {
    auto p = new Byte(value);
    p->accept();
    if(is_byte_to_large(p)) {
        errorByteTooLarge(yylineno, to_string(*p->value));
        exit(1);
    }
    return p;
}

Cast* MakeCast(struct Type *toType, struct Exp *exp) {
    if(!is_castable(toType->typeCase, exp->type->typeCase, true)) {
        errorMismatch(yylineno);
        exit(1);
    }
    auto p = new Cast(toType, exp);
    p->accept();
    return p;
}

LogicOp* MakeLogicOp(struct Exp *r_value, struct Exp *l_value) {
    auto p = new LogicOp(r_value, l_value);
    p->accept();
    return p;
}

And* MakeAnd(struct Exp *lValue, AndMarker* marker, struct Exp *rValue) {
    if(!is_bool(lValue) && !is_bool(rValue)) {
        errorMismatch(yylineno);
        exit(1);
    }
    auto p = new And(lValue, rValue, marker);
    p->accept();
    return p;
}
OrMarker* MakeOrMarker() {
    auto p = new OrMarker();
    p->accept();
    return p;
}
AndMarker* MakeAndMarker() {
    auto p = new AndMarker();
    p->accept();
    return p;
}
Or* MakeOr(struct Exp *l_value, OrMarker* marker, struct Exp *r_value) {
    if(!is_bool(l_value) && !is_bool(r_value)) {
        errorMismatch(yylineno);
        exit(1);
    }
    marker->accept();
    auto p = new Or(l_value, r_value, marker);
    p->accept();
    return p;
}

Not* MakeNot(struct Exp *value) {
    if(!is_bool(value)) {
        errorMismatch(yylineno);
        exit(1);
    }
    auto p = new Not(value);
    p->accept();
    return p;
}

Binop* MakeBinop(struct Exp *l_exp, struct Exp *r_exp, BinopCase binopCase) {
    auto p = new Binop(l_exp, r_exp, binopCase);
    if(!p->is_legal(yylineno)) {
        exit(1);
    }
    p->accept();
    return p;
}

Binop* MakeBinop(struct Exp *l_exp, struct Exp *r_exp, struct Type *type) {
    auto p = new Binop(l_exp, r_exp, type);
    p->accept();
    return p;
}

Relop* MakeRelop(struct Exp *l_exp, struct Exp *r_exp, RelopCase relopCase) {
    auto p = new Relop(l_exp, r_exp, relopCase);
    if(!p->is_legal(yylineno)) {
        exit(1);
    }
    p->accept();
    return p;
}

String* MakeString(string value) {
    auto p = new String(value);
    p->accept();
    return p;
}

Trinari* MakeTrinari(struct Exp *true_exp, struct Exp *cond_exp, struct Exp *false_exp) {
    if(!is_type_compatible(true_exp, false_exp)) {
        errorMismatch(yylineno);
        exit(1);
    }
    auto p = new Trinari(true_exp, cond_exp, false_exp);
    p->accept();
    return p;
}

Bool* MakeBool(bool value) {
    auto p = new Bool(new bool(value));
    p->accept();
    return p;
}

Statement *MakeStatement(Statement &ref) {
    Statement *p = new Statement(ref);
    p->accept();
    return p;
}

Declaration* MakeDeclaration(struct Type *type, struct Id *id) {
    if(is_id_declared(&store, id)) {
        errorDef(yylineno, id->id);
        exit(1);
    }
    id->type = type;
    insert_id(&store, id);
    auto p = new Declaration(type, id);
    p->accept();
    return p;
}

Assignment* MakeAssignment(struct Type *type, struct Id *id, struct Exp *value) {
    if(is_id_declared(&store, id)) {
        errorDef(yylineno, id->id);
        exit(1);
    }
    id->type = type;
    if(!is_type_compatible(id, value)) {
        errorMismatch(yylineno);
        exit(1);
    }
    if(has_value(value)) {
        id->value = value->value;
    }
    insert_id(&store, id);
    auto p = new Assignment(type, id, value);
    p->accept();
    return p;
}

LateAssignment* MakeLateAssignment(struct Id *id, struct Exp *value) {
    if(!is_id_declared(&store, id)) {
        errorUndef(yylineno, id->id);
        exit(1);
    }
    Id* retId = get_id(&store, id);
    if(id == nullptr) {
        errorUndef(yylineno, retId->id);
    }
    assert(id == retId);
    if(!is_type_compatible(id, value)) {
        errorMismatch(yylineno);
        exit(1);
    }
    id->type = value->type;
    id->value = value->value;
    auto p = new LateAssignment(id, value);
    p->accept();
    return p;
}

Call* MakeCall(struct CallExp *callExp) {
    auto p = new Call(callExp);
    p->accept();
    if(!is_call_correct(&store, p, yylineno)) {
        exit(1);
    }
    return p;
}

FunctionCall* MakeFunctionCall(struct Call *call) {
    auto p = new FunctionCall(call);
    p->accept();
    return p;
}

FunctionCall* MakeFunctionCall(struct Id *function_id, struct ExpList *args) {
    auto p = new FunctionCall(function_id, args);
    p->accept();
    return p;
}

Return* MakeReturn() {
    if(store.get_current_func_ret_type() != Type::VOID) {
        errorMismatch(yylineno);
        exit(1);
    }
    auto p = new Return();
    p->accept();
    return p;
}

Return* MakeReturn(struct Exp *value) {
    if(!is_castable(value->type->typeCase, store.get_current_func_ret_type())) {
        errorMismatch(yylineno);
        exit(1);
    }
    auto p = new Return(value);
    p->accept();
    return p;
}

If_pattern* MakeIf_pattern(struct Exp *condition, struct Statement *stmnt) {
    auto p = new If_pattern(condition, stmnt);
    p->accept();
    return p;
}

If_pattern* MakeIf_pattern(struct Exp *condition, struct Statement *true_stmnt, struct Statement *false_stmnt) {
    auto p = new If_pattern(condition, true_stmnt, false_stmnt);
    p->accept();
    return p;
}

While* MakeWhile(struct Exp *condition, struct Statement *stmnt) {
    end_while(&store);
    auto p = new While(condition, stmnt);
    p->accept();
    return p;
}

Statements* MakeStatements() {
    auto p = new Statements();
    p->accept();
    return p;
}

Statements* MakeStatements(struct Statement *stmnt1) {
    auto p = new Statements(stmnt1);
    p->accept();
    return p;
}

Statements* MakeStatements(struct Statements *stmnts, struct Statement *stmnt) {
    stmnts->statement_list.push_back(stmnt);
    return stmnts;
}

RetType *MakeRetType(Type::TypeCase type) {
    begin_function(&store, type);
    auto *p = new RetType(type);
    p->accept();
    return p;
}

RetType *MakeRetType(Type *type) {
    begin_function(&store, type->typeCase);
    auto *p = new RetType(type);
    p->accept();
    return p;
}

FormalDecl* MakeFormalDecl(Type* type, Id *id) {
    if(is_id_declared(&store, id)) {
        errorDef(yylineno, id->id);
        exit(1);
    }
    id->type = type;
    auto p = new FormalDecl(id);
    p->accept();
    return p;
}


FormalsList *MakeFormalsList(FormalDecl* formals) {
    auto *p = new FormalsList(formals);
    p->accept();
    return p;
}
FormalsList *MakeFormalsList(FormalDecl* formals, FormalsList* formalsList) {
    if(is_formal_declared(formalsList->formalList, formals->id)){
        errorDef(yylineno, formals->id->id);
        exit(1);
    }
    formalsList->formalList.insert(formalsList->formalList.begin(), formals);
    auto *p = new FormalsList(formals);
    p->accept();
    return p;
}

Formals *MakeFormals() {
    auto *p = new Formals();
    p->accept();
    return p;
}

Formals *MakeFormals(FormalsList* formals) {
    insert_function_params(&store, formals);
    auto *p = new Formals(formals);
    p->accept();
    return p;
}

FuncDecl *MakeFuncDecl(RetType* retType, Id *id, Formals *formals, Statements *stmts) {
    id->type = retType->type;
    FuncDecl *p = new FuncDecl(id, formals, stmts);
    p->accept();
    end_function(&store, p);
    return p;
}

Funcs *MakeFuncs() {
    Funcs *p = new Funcs();
    p->accept();
    return p;
}
Funcs *MakeFuncs(FuncDecl* func, Funcs* funcs) {
    funcs->func_list.push_back(func);
    return funcs;
}

Program *MakeProgram(struct Funcs *funcs) {
    auto *p = new Program(funcs);
    p->accept();
    return p;
}
Exp* MakeBoolExp(Exp* exp) {
    if(!validations::is_bool(exp)) {
        errorMismatch(yylineno);
        exit(1);
    }
    exp->is_bool = true;
    exp->accept();
    return exp;
}
Break* MakeBreak() {
    if(!store.get_in_while()) {
        errorUnexpectedBreak(yylineno);
        exit(1);
    }
    auto p = new Break();
    p->accept();
    return p;
}

Continue* MakeContinue() {
    if(!store.get_in_while()) {
        errorUnexpectedContinue(yylineno);
        exit(1);
    }
    auto p = new Continue();
    p->accept();
    return p;
}
InitMarker* MakeInitMarker() {
    auto p = new InitMarker();
    p->accept();
    return p;
}
EndMarker* MakeEndMarker() {
    auto p = new EndMarker();
    p->accept();
    return p;
}