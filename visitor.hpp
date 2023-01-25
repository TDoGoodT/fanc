//
// Created by snir on 1/25/23.
//

#ifndef ANALYZER_VISITOR_HPP
#define ANALYZER_VISITOR_HPP


#include "types.hpp"
#include "bp.hpp"

class Visitor {
    CodeBuffer& codeBuffer = CodeBuffer::instance();
public:
    static Visitor& instance();

    void visit(const _T_Exp *element);

    void visit(const _T_Number *element);

    void visit(const _T_Declaration *element);

    void visit(const _T_Assignment *element);

    void visit(const _T_LateAssignment *element);

    void visit(const _T_FunctionCall *element);

    void visit(const _T_Return *element);

    void visit(const _T_If_pattern *element);

    void visit(const _T_While *element);

    void visit(const _T_Statements *element);

    void visit(const _T_Statement *element);

    void visit(const _T_Not *element);

    void visit(const _T_Assign *element);

    void visit(const _T_If *element);

    void visit(const _T_Else *element);

    void visit(const _T_Id *element);

    void visit(const _T_String *element);

    void visit(const _T_Cast *element);

    void visit(const _T_Void *element);

    void visit(const _T_Bool *element);

    void visit(const _T_RetType *element);

    void visit(const _T_Program *element);

    void visit(const _T_Funcs *element);

    void visit(const _T_FuncDecl *element);

    void visit(const _T_Formals *element);

    void visit(const _T_FormalsList *element);

    void visit(const _T_FormalDecl *element);

    void visit(const _T_ExpList *element);

    void visit(const _T_Binop *element);

    void visit(const _T_Trinari *element);

    void visit(const _T_Call *element);

    void visit(const _T_CallExp *element);
};


#endif //ANALYZER_VISITOR_HPP
