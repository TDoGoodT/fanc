//
// Created by snir on 1/25/23.
//

#include "visitor.hpp"

Visitor &Visitor::instance() {
    static Visitor inst;//only instance
    return inst;
}

void Visitor::visit(const _T_Binop *element) {
    string op_str;
    switch (element->binopCase) {
        case _T_Binop::_PLUS_:
            op_str = "add";
            break;
        case _T_Binop::_MINUS_:
            op_str = "sub";
            break;
        case _T_Binop::_MULT_:
            op_str = "mul";
            break;
        case _T_Binop::_DIV_:
            // TODO: check for division by zero
            op_str = "div";
            break;
    }
    element->place = codeBuffer.newTemp();
    // TODO: handle overflow
    codeBuffer.emit(element->place + ":=" + element->r_exp->place + op_str + element->l_exp->place);
}

void Visitor::visit(const _T_Exp *element) {

}

void Visitor::visit(const _T_Number *element) {

}

void Visitor::visit(const _T_Declaration *element) {

}

void Visitor::visit(const _T_Assignment *element) {

}

void Visitor::visit(const _T_LateAssignment *element) {

};

void Visitor::visit(const _T_FunctionCall *element) {

}

void Visitor::visit(const _T_Return *element) {

}

void Visitor::visit(const _T_If_pattern *element) {

}

void Visitor::visit(const _T_While *element) {

}

void Visitor::visit(const _T_Statements *element) {

}

void Visitor::visit(const _T_Statement *element) {

}

void Visitor::visit(const _T_Not *element) {

}

void Visitor::visit(const _T_Assign *element) {

}

void Visitor::visit(const _T_If *element) {

}

void Visitor::visit(const _T_Else *element) {

}

void Visitor::visit(const _T_Id *element) {

}

void Visitor::visit(const _T_String *element) {

}

void Visitor::visit(const _T_Cast *element) {

}

void Visitor::visit(const _T_Void *element) {

}

void Visitor::visit(const _T_Bool *element) {

}

void Visitor::visit(const _T_RetType *element) {

}

void Visitor::visit(const _T_Program *element) {

}

void Visitor::visit(const _T_Funcs *element) {

}

void Visitor::visit(const _T_FuncDecl *element) {

}

void Visitor::visit(const _T_Formals *element) {

}

void Visitor::visit(const _T_FormalsList *element) {

}

void Visitor::visit(const _T_FormalDecl *element) {

}

void Visitor::visit(const _T_ExpList *element) {

}


void Visitor::visit(const _T_Trinari *element) {

}

void Visitor::visit(const _T_Call *element) {

}

void Visitor::visit(const _T_CallExp *element) {

}