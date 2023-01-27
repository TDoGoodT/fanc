#include "types.hpp"

//
// Created by Snir Bachar on 27/01/2023.
//
_T_Type* MakeType(_T_Type::Type type);

_T_Exp* MakeExp(struct _T_Type *type);

_T_Exp* MakeExp(struct _T_Type *type, int *value);

_T_Exp* MakeExp(struct _T_Type *type, int *value, bool in_statement);

_T_ExpList *MakeExpList();

_T_ExpList *MakeExpList(_T_Exp *exp);

_T_Id* MakeId(string id, struct _T_Type *type);

_T_CallExp* MakeCallExp(struct _T_Id *function);

_T_CallExp* MakeCallExp(struct _T_Id *function, struct _T_ExpList *args);

_T_Number* MakeNumber(int *value, struct _T_Type *type);

_T_Int* MakeInt(int *value);

_T_Byte* MakeByte(int *value);

_T_Cast* MakeCast(struct _T_Type *toType, struct _T_Exp *exp);

_T_LogicOp* MakeLogicOp(struct _T_Exp *r_value, struct _T_Exp *l_value);

_T_And* MakeAnd(struct _T_Exp *l_value, struct _T_Exp *r_value);

_T_Or* MakeOr(struct _T_Exp *l_value, struct _T_Exp *r_value);

_T_Not* MakeNot(struct _T_Exp *value);

_T_Binop* MakeBinop(struct _T_Exp *l_exp, struct _T_Exp *r_exp, _T_Binop::BinopCase binopCase);

_T_Binop* MakeBinop(struct _T_Exp *l_exp, struct _T_Exp *r_exp, struct _T_Type *type);

_T_Relop* MakeRelop(struct _T_Exp *l_exp, struct _T_Exp *r_exp, RelopCase relopCase);

_T_String* MakeString(string value);

_T_Trinari* MakeTrinari(struct _T_Exp *true_exp, struct _T_Exp *cond_exp, struct _T_Exp *false_exp);

_T_Bool* MakeBool(bool value);

_T_Statement *MakeStatement(_T_Statement &ref);

_T_Declaration* MakeDeclaration(struct _T_Type *type, struct _T_Id *id);

_T_Assignment* MakeAssignment(struct _T_Type *type, struct _T_Id *id, struct _T_Exp *value);

_T_LateAssignment* MakeLateAssignment(struct _T_Id *id, struct _T_Exp *value);

_T_Call* MakeCall(struct _T_CallExp *callExp);

_T_FunctionCall* MakeFunctionCall(struct _T_Call *call);

_T_FunctionCall* MakeFunctionCall(struct _T_Id *function_id, struct _T_ExpList *args);

_T_Return* MakeReturn();

_T_Return* MakeReturn(struct _T_Exp *value);

_T_If_pattern* MakeIf_pattern(struct _T_Exp *condition, struct _T_Statement *stmnt);

_T_If_pattern* MakeIf_pattern(struct _T_Exp *condition, struct _T_Statement *true_stmnt, struct _T_Statement *false_stmnt);

_T_While* MakeWhile(struct _T_Exp *condition, struct _T_Statement *stmnt);

_T_Statements* MakeStatements();

_T_Statements* MakeStatements(struct _T_Statement *stmnt1);

_T_Statements* MakeStatements(struct _T_Statement *stmnt1, struct _T_Statement *stmnt2);

_T_RetType *MakeRetType(_T_Type::Type type);

_T_RetType *MakeRetType(_T_Type *type);

_T_FormalDecl* MakeFormalDecl(struct _T_Id *id);

_T_FormalsList *MakeFormalsList();

_T_FormalsList *MakeFormalsList(_T_FormalDecl* formals);

_T_Formals *MakeFormals();

_T_Formals *MakeFormals(_T_FormalsList* formals);

_T_FuncDecl *MakeFuncDecl(struct _T_Id *id, struct _T_Formals *formals, struct _T_Statements *stmts);

_T_Funcs *MakeFuncs();

_T_Program *MakeProgram(struct _T_Funcs *funcs);

_T_Break* MakeBreak();

_T_Continue* MakeContinue();
