//
// Created by snir on 1/25/23.
//

#include <cassert>
#include "visitor.hpp"
CodeBuffer& codeBuffer = CodeBuffer::instance();

Visitor &Visitor::instance() {
    static Visitor inst;//only instance
    return inst;
}

string get_llvm_type(_T_Type *pType) {
    switch(pType->typeCase) {
        case _T_Type::_INT_:
            return "i32";
            break;
        case _T_Type::_BOOL_:
            return "i1";
            break;
        case _T_Type::_BYTE_:
            return "i8";
            break;
        case _T_Type::_STRING_:
        case _T_Type::_VOID_:
            assert(false);
            break;
    }
    exit(0);
}

string get_llvm_bin_op(_T_Binop::BinopCase binopCase) {
    switch (binopCase) {
        case _T_Binop::_PLUS_:
            return "add";
            break;
        case _T_Binop::_MINUS_:
            return "sub";
            break;
        case _T_Binop::_MULT_:
            return "mul";
            break;
        case _T_Binop::_DIV_:
            return "div";
            break;
    }
    exit(0);
}
void Visitor::visit(const _T_Binop *element) {
    string op_str = get_llvm_bin_op(element->binopCase);
    string r_str = element->r_exp->place;
    string l_str = element->l_exp->place;
    if(r_str.empty()) {
        assert(element->r_exp->value != nullptr);
        r_str =  get_llvm_type(element->r_exp->type) + " " + to_string(*element->r_exp->value);
    }
    if(l_str.empty()) {
        assert(element->l_exp->value != nullptr);
        l_str =  get_llvm_type(element->l_exp->type) + " " + to_string(*element->l_exp->value);
    }
    element->place = codeBuffer.newTemp();
    emitBinop(element, op_str, r_str, l_str);
}

void Visitor::emitMaskTargetByte(const string& place) { codeBuffer.emit("andi " + place + ", 0xff"); }

void Visitor::emitBinop(const _T_Binop *element, const string &op_str, const string &r_str, const string &l_str) {
    if(element->binopCase == _T_Binop::_DIV_) {
        emitDivByZeroHandler(r_str);
    }
    codeBuffer.emit(element->place + " = " + op_str  + " " + l_str + ", " + r_str);
    if(element->type->typeCase == _T_Type::_BYTE_) {
        emitMaskTargetByte(element->place);
    }
}

void Visitor::emitDivByZeroHandler(const string &r_str) { codeBuffer.emit("beq " + r_str + ", 0, div_by_zero_handler"); }

void Visitor::visit(const _T_Exp *element) {

}

void Visitor::visit(const _T_Number *element) {

}

void Visitor::visit(const _T_Declaration *element) {
    element->place = codeBuffer.newTemp();
    codeBuffer.emit(element->place + " = alloca " + get_llvm_type(element->type));
}

void Visitor::visit(const _T_Assignment *element) {
    element->id->place = codeBuffer.newTemp();
    if (element->value->value != nullptr && element->value->place.empty()) {
        assert(element->value->type != nullptr);
        codeBuffer.emit(element->id->place + " = " + get_llvm_type(element->value->type) + " " + to_string(*element->value->value));
        return;
    }
    assert(not element->value->place.empty());
    codeBuffer.emit(element->id->place + " = " + element->value->place);
}

void Visitor::visit(const _T_LateAssignment *element) {
    assert(not element->place.empty());
    codeBuffer.emit(element->id->place + " = store " + element->place + " " + element->value->place);
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
    element->place = codeBuffer.newTemp();
    assert(not element->value->place.empty());
    codeBuffer.emit(element->place + " = xor " + get_llvm_type(element->value->type) + " " + element->value->place + " 1");
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

void Visitor::visit(const InitMarker *element) {
    codeBuffer.emitGlobal("str_0: .asciiz \"Error division by zero\"");
    codeBuffer.emitGlobal("str_1: .asciiz \"Error index out of bounds\"");
    codeBuffer.emitGlobal("str_2: .asciiz \"\\n\"");
    codeBuffer.emitLabel("_printi:");
    codeBuffer.emit("    lw $a0,0($sp)");
    codeBuffer.emit("    li $v0,1");
    codeBuffer.emit("    syscall");
    codeBuffer.emit("    jr $ra");
    codeBuffer.emitLabel(".end printi\n");
    codeBuffer.emitLabel("_print:");
    codeBuffer.emit("    lw $a0,0($sp)");
    codeBuffer.emit("    li $v0,4");
    codeBuffer.emit("    syscall");
    codeBuffer.emit("    jr $ra");
    codeBuffer.emitLabel(".end print\n");
    codeBuffer.emitLabel("div_by_zero_handler:");
    codeBuffer.emit("    la $a0,str_0");
    codeBuffer.emit("    li $v0,4");
    codeBuffer.emit("    syscall");
    codeBuffer.emit("    la $a0,str_2");
    codeBuffer.emit("    li $v0,4");
    codeBuffer.emit("    syscall");
    codeBuffer.emit("    li $v0,10");
    codeBuffer.emit("    syscall");
    codeBuffer.emitLabel(".end div_by_zero_handler\n");
    codeBuffer.emitLabel("out_of_bounds_handler:");
    codeBuffer.emit("    la $a0,str_1");
    codeBuffer.emit("    li $v0,4");
    codeBuffer.emit("    syscall");
    codeBuffer.emit("    la $a0,str_2");
    codeBuffer.emit("    li $v0,4");
    codeBuffer.emit("    syscall");
    codeBuffer.emit("    li $v0,10");
    codeBuffer.emit("    syscall");
    codeBuffer.emitLabel(".end out_of_bounds_handler\n");
}