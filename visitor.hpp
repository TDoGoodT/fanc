//
// Created by snir on 1/25/23.
//

#ifndef ANALYZER_VISITOR_HPP
#define ANALYZER_VISITOR_HPP


#include "types.hpp"
#include "markers.hpp"
#include "bp.hpp"

class Visitor {
public:
    static Visitor& instance();

    static void visit(const struct _T_Exp *element);

    static void visit(const struct _T_Number *element);

    static void visit(const struct _T_Declaration *element);

    static void visit(const struct _T_Assignment *element);

    static void visit(const struct _T_LateAssignment *element);

    static void visit(const struct _T_FunctionCall *element);

    static void visit(const struct _T_Return *element);

    static void visit(const struct _T_If_pattern *element);

    static void visit(const struct _T_While *element);

    static void visit(const struct _T_Statements *element);

    static void visit(const struct _T_Statement *element);

    static void visit(const struct _T_Not *element);

    static void visit(const struct _T_If *element);

    static void visit(const struct _T_Else *element);

    static void visit(const struct _T_Id *element);

    static void visit(const struct _T_String *element);

    static void visit(const struct _T_Cast *element);

    static void visit(const struct _T_Void *element);

    static void visit(const struct _T_Bool *element);

    static void visit(const struct _T_RetType *element);

    static void visit(const struct _T_Program *element);

    static void visit(const struct _T_Funcs *element);

    static void visit(const struct _T_FuncDecl *element);

    static void visit(const struct _T_Formals *element);

    static void visit(const struct _T_FormalsList *element);

    static void visit(const struct _T_FormalDecl *element);

    static void visit(const struct _T_ExpList *element);

    static void visit(const struct _T_Binop *element);

    static void visit(const struct _T_Trinari *element);

    static void visit(const struct _T_Call *element);

    static void visit(const struct _T_CallExp *element);

    static void visit(const struct InitMarker *element);

    static void emitDivByZeroHandler(const string &r_str);

    static void emitBinop(const _T_Binop *element, const string &op_str, const string &r_str, const string &l_str);

    static void emitMaskTargetByte(const string& place);
};


#endif //ANALYZER_VISITOR_HPP
