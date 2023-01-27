#include "markers.hpp"

//
// Created by Snir Bachar on 27/01/2023.
//
_T_Type* MakeType(_T_Type::Type type) {
    return new _T_Type(type);
}

_T_Exp* MakeExp(struct _T_Type *type) {
    _T_Exp *p = new _T_Exp(type);
    p->accept();
    return p;

}

_T_Exp* MakeExp(struct _T_Type *type, int *value) {
    _T_Exp *p = new _T_Exp(type, value);
    p->accept();
    return p;

}

_T_Exp* MakeExp(struct _T_Type *type, int *value, bool in_statement) {
    _T_Exp *p = new _T_Exp(type, value, in_statement);
    p->accept();
    return p;

}

_T_ExpList *MakeExpList() {
    _T_ExpList *p = new _T_ExpList();
    p->accept();
    return p;
}

_T_ExpList *MakeExpList(_T_Exp *exp) {
    _T_ExpList *p = new _T_ExpList(exp);
    p->accept();
    return p;
}

_T_Id* MakeId(string id, struct _T_Type *type) {
    auto p = new _T_Id(id, type);
    p->accept();
    return p;
}

_T_CallExp* MakeCallExp(struct _T_Id *function) {
    auto p = new _T_CallExp(function);
    p->accept();
    return p;
}

_T_CallExp* MakeCallExp(struct _T_Id *function, struct _T_ExpList *args) {
    auto p = new _T_CallExp(function, args);
    p->accept();
    return p;
}

_T_Number* MakeNumber(int *value, struct _T_Type *type) {
    auto p = new _T_Number(value, type);
    p->accept();
    return p;
}

_T_Int* MakeInt(int *value) {
    auto p = new _T_Int(value);
    p->accept();
    return p;
}

_T_Byte* MakeByte(int *value) {
    auto p = new _T_Byte(value);
    p->accept();
    return p;
}

_T_Cast* MakeCast(struct _T_Type *toType, struct _T_Exp *exp) {
    auto p = new _T_Cast(toType, exp);
    p->accept();
    return p;
}

_T_LogicOp* MakeLogicOp(struct _T_Exp *r_value, struct _T_Exp *l_value) {
    auto p = new _T_LogicOp(r_value, l_value);
    p->accept();
    return p;
}

_T_And* MakeAnd(struct _T_Exp *l_value, struct _T_Exp *r_value) {
    auto p = new _T_And(l_value, r_value);
    p->accept();
    return p;
}

_T_Or* MakeOr(struct _T_Exp *l_value, struct _T_Exp *r_value) {
    auto p = new _T_Or(l_value, r_value);
    p->accept();
    return p;
}

_T_Not* MakeNot(struct _T_Exp *value) {
    auto p = new _T_Not(value);
    p->accept();
    return p;
}

_T_Binop* MakeBinop(struct _T_Exp *l_exp, struct _T_Exp *r_exp, _T_Binop::BinopCase binopCase) {
    auto p = new _T_Binop(l_exp, r_exp, binopCase);
    p->accept();
    return p;
}

_T_Binop* MakeBinop(struct _T_Exp *l_exp, struct _T_Exp *r_exp, struct _T_Type *type) {
    auto p = new _T_Binop(l_exp, r_exp, type);
    p->accept();
    return p;
}

_T_Relop* MakeRelop(struct _T_Exp *l_exp, struct _T_Exp *r_exp, RelopCase relopCase) {
    auto p = new _T_Relop(l_exp, r_exp, relopCase);
    p->accept();
    return p;
}

_T_String* MakeString(string value) {
    auto p = new _T_String(value);
    p->accept();
    return p;
}

_T_Trinari* MakeTrinari(struct _T_Exp *true_exp, struct _T_Exp *cond_exp, struct _T_Exp *false_exp) {
    auto p = new _T_Trinari(true_exp, cond_exp, false_exp);
    p->accept();
    return p;
}

_T_Bool* MakeBool(bool value) {
    auto p = new _T_Bool(value);
    p->accept();
    return p;
}

_T_Statement *MakeStatement(_T_Statement &ref) {
    _T_Statement *p = new _T_Statement(ref);
    p->accept();
    return p;
}

_T_Declaration* MakeDeclaration(struct _T_Type *type, struct _T_Id *id) {
    auto p = new _T_Declaration(type, id);
    p->accept();
    return p;
}

_T_Assignment* MakeAssignment(struct _T_Type *type, struct _T_Id *id, struct _T_Exp *value) {
    auto p = new _T_Assignment(type, id, value);
    p->accept();
    return p;
}

_T_LateAssignment* MakeLateAssignment(struct _T_Id *id, struct _T_Exp *value) {
    auto p = new _T_LateAssignment(id, value);
    p->accept();
    return p;
}

_T_Call* MakeCall(struct _T_CallExp *callExp) {
    auto p = new _T_Call(callExp);
    p->accept();
    return p;
}

_T_FunctionCall* MakeFunctionCall(struct _T_Call *call) {
    auto p = new _T_FunctionCall(call);
    p->accept();
    return p;
}

_T_FunctionCall* MakeFunctionCall(struct _T_Id *function_id, struct _T_ExpList *args) {
    auto p = new _T_FunctionCall(function_id, args);
    p->accept();
    return p;
}

_T_Return* MakeReturn() {
    auto p = new _T_Return();
    p->accept();
    return p;
}

_T_Return* MakeReturn(struct _T_Exp *value) {
    auto p = new _T_Return(value);
    p->accept();
    return p;
}

_T_If_pattern* MakeIf_pattern(struct _T_Exp *condition, struct _T_Statement *stmnt) {
    auto p = new _T_If_pattern(condition, stmnt);
    p->accept();
    return p;
}

_T_If_pattern* MakeIf_pattern(struct _T_Exp *condition, struct _T_Statement *true_stmnt, struct _T_Statement *false_stmnt) {
    auto p = new _T_If_pattern(condition, true_stmnt, false_stmnt);
    p->accept();
    return p;
}

_T_While* MakeWhile(struct _T_Exp *condition, struct _T_Statement *stmnt) {
    auto p = new _T_While(condition, stmnt);
    p->accept();
    return p;
}

_T_Statements* MakeStatements() {
    auto p = new _T_Statements();
    p->accept();
    return p;
}

_T_Statements* MakeStatements(struct _T_Statement *stmnt1) {
    auto p = new _T_Statements(stmnt1);
    p->accept();
    return p;
}

_T_Statements* MakeStatements(struct _T_Statement *stmnt1, struct _T_Statement *stmnt2) {
    auto p = new _T_Statements(stmnt1, stmnt2);
    p->accept();
    return p;
}

_T_RetType *MakeRetType(_T_Type::Type type) {
    _T_RetType *p = new _T_RetType(type);
    p->accept();
    return p;
}

_T_RetType *MakeRetType(_T_Type *type) {
    _T_RetType *p = new _T_RetType(type);
    p->accept();
    return p;
}

_T_FormalDecl* MakeFormalDecl(struct _T_Id *id) {
    auto p = new _T_FormalDecl(id);
    p->accept();
    return p;
}

_T_FormalsList *MakeFormalsList() {
    _T_FormalsList *p = new _T_FormalsList();
    p->accept();
    return p;
}

_T_FormalsList *MakeFormalsList(_T_FormalDecl* formals) {
    auto *p = new _T_FormalsList(formals);
    p->accept();
    return p;
}

_T_Formals *MakeFormals() {
    auto *p = new _T_Formals();
    p->accept();
    return p;
}

_T_Formals *MakeFormals(_T_FormalsList* formals) {
    auto *p = new _T_Formals(formals);
    p->accept();
    return p;
}

_T_FuncDecl *MakeFuncDecl(struct _T_Id *id, struct _T_Formals *formals, struct _T_Statements *stmts) {
    _T_FuncDecl *p = new _T_FuncDecl(id, formals, stmts);
    p->accept();
    return p;
}

_T_Funcs *MakeFuncs() {
    _T_Funcs *p = new _T_Funcs();
    p->accept();
    return p;
}

_T_Program *MakeProgram(struct _T_Funcs *funcs) {
    auto *p = new _T_Program(funcs);
    p->accept();
    return p;
}

_T_Break* MakeBreak() {
    auto p = new _T_Break();
    p->accept();
    return p;
}

_T_Continue* MakeContinue() {
    auto p = new _T_Continue();
    p->accept();
    return p;
}
