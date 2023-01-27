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

string to_string(_T_Type *pType) {
    switch(pType->typeCase) {
        case _T_Type::_INT_:
            return "i32";
        case _T_Type::_BOOL_:
            return "i1";
        case _T_Type::_BYTE_:
            return "i8";
        case _T_Type::_STRING_:
        case _T_Type::_VOID_:
            assert(false);
    }
    exit(0);
}

string get_llvm_bin_op(_T_Binop::BinopCase binopCase) {
    switch (binopCase) {
        case _T_Binop::_PLUS_:
            return "add";
        case _T_Binop::_MINUS_:
            return "sub";
        case _T_Binop::_MULT_:
            return "mul";
        case _T_Binop::_DIV_:
            return "div";
    }
    exit(0);
}
void Visitor::visit(_T_Binop *element) {
    string op_str = get_llvm_bin_op(element->binopCase);
    string r_str = element->r_exp->place;
    string l_str = element->l_exp->place;
    if(r_str.empty()) {
        assert(element->r_exp->value != nullptr);
        r_str =  to_string(*element->r_exp->value);
    }
    if(l_str.empty()) {
        assert(element->l_exp->value != nullptr);
        l_str = to_string(element->l_exp->type) + " " + to_string(*element->l_exp->value);
    }
    element->place = codeBuffer.newTemp();
    emitBinop(element, op_str, r_str, l_str);
}

void Visitor::emitMaskTargetByte(string& place) { codeBuffer.emit("andi " + place + ", 0xff"); }

void Visitor::emitBinop(_T_Binop *element, string &op_str, string &r_str, string &l_str) {
    if(element->binopCase == _T_Binop::_DIV_) {
        emitDivByZeroHandler(r_str);
    }
    codeBuffer.emit(element->place + " = " + op_str  + " " + l_str + ", " + r_str);
    if(element->type->typeCase == _T_Type::_BYTE_) {
        emitMaskTargetByte(element->place);
    }
}

void Visitor::emitDivByZeroHandler(string &r_str) { codeBuffer.emit("beq " + r_str + ", 0, div_by_zero_handler"); }

void Visitor::visit(_T_Exp *element) {

}

void Visitor::visit(_T_Or *element) {
    string r_str = element->r_value->place;
    string l_str = element->l_value->place;
    element->place = codeBuffer.newTemp();
    int line = codeBuffer.getLineNumber();
    element->trueList = CodeBuffer::makelist({line, FIRST});
    element->falseList = CodeBuffer::makelist({line, SECOND});

}
void Visitor::visit(_T_And *element) {
    string r_str = element->r_value->place;
    string l_str = element->l_value->place;
    element->place = codeBuffer.newTemp();
    int line = codeBuffer.getLineNumber();
    element->trueList = CodeBuffer::makelist({line, FIRST});
    element->falseList = CodeBuffer::makelist({line, SECOND});
}

void Visitor::visit(_T_Number *element) {

}

void Visitor::visit(_T_Declaration *element) {
    element->place = codeBuffer.newTemp();
    codeBuffer.emit(element->place + " = add " + to_string(element->type) + " 0, 0" );
}

void Visitor::visit(_T_Assignment *element) {
    string code;
    element->id->place = codeBuffer.newTemp();
    code += element->id->place + " = add ";
    if (element->value->value != nullptr && element->value->place.empty()) {
        assert(element->value->type != nullptr);
        code += to_string(element->value->type) + " " + to_string(*element->value->value) + ", 0";
    } else {
        assert(not element->value->place.empty());
        code += to_string(element->value->type) + " " + element->value->place + ", 0";

    }
    codeBuffer.emit(code);
}

void Visitor::visit(_T_LateAssignment *element) {
    assert(not element->place.empty());
    codeBuffer.emit(element->id->place + " = store " + element->place + " " + element->value->place);
};

void Visitor::visit(_T_FunctionCall *element) {

}

void Visitor::visit(_T_Return *element) {

}

void Visitor::visit(_T_If_pattern *element) {

}

void Visitor::visit(_T_While *element) {

}

void Visitor::visit(_T_Statements *element) {

}

void Visitor::visit(_T_Statement *element) {

}

void Visitor::visit(_T_Not *element) {
    element->place = codeBuffer.newTemp();
    assert(not element->value->place.empty());
    codeBuffer.emit(element->place + " = xor " + to_string(element->value->type) + " " + element->value->place + ", 1");
}


void Visitor::visit(_T_Relop *element) {
    string label = codeBuffer.genLabel();
    element->place = codeBuffer.newTemp();
    string op_str = getLlvmRelop(element->relopCase);
    int line = codeBuffer.getLineNumber();
    element->trueList = CodeBuffer::makelist({line, FIRST});
    element->falseList = CodeBuffer::makelist({line, SECOND});
}

void Visitor::visit(_T_If *element) {

}

static map<RelopCase, string> relopToLlvm = {
    { RelopCase::_GE_, "sge" },
    { RelopCase::_GT_, "sgt" },
    { RelopCase::_LE_, "sle" },
    { RelopCase::_LT_, "slt" },
    { RelopCase::_EQ_, "eq" },
    { RelopCase::_NE_, "ne" }
};

string Visitor::getLlvmRelop(RelopCase relopCase) {
    return relopToLlvm[relopCase];
}

void Visitor::visit(_T_Else *element) {

}

void Visitor::visit(_T_Id *element) {

}

void Visitor::visit(_T_String *element) {

}

void Visitor::visit(_T_Cast *element) {

}

void Visitor::visit(_T_Void *element) {

}

void Visitor::visit(_T_Bool *element) {
    element->place = codeBuffer.newTemp();
    assert(element->type != nullptr && element->type->typeCase == _T_Type::_BOOL_);
    codeBuffer.emit(element->place + " = add " + to_string(element->type) + " " + (element->value ? "1" : "0") + ", 0");
}

void Visitor::visit(_T_RetType *element) {

}

void Visitor::visit(_T_Program *element) {

}

void Visitor::visit(_T_Funcs *element) {

}

void Visitor::visit(_T_FuncDecl *element) {

}

void Visitor::visit(_T_Formals *element) {

}

void Visitor::visit(_T_FormalsList *element) {

}

void Visitor::visit(_T_FormalDecl *element) {

}

void Visitor::visit(_T_ExpList *element) {

}


void Visitor::visit(_T_Trinari *element) {

}

void Visitor::visit(_T_Call *element) {

}

void Visitor::visit(_T_CallExp *element) {

}

void Visitor::visit(InitMarker *element) {
    string globals[] = {
            "declare i32 @printf(i8*, ...)",
            "declare void @exit(i32)",
            R"(@.int_specifier = constant [4 x i8] c"%d\0A\00")",
            R"(@.str_specifier = constant [4 x i8] c"%s\0A\00")",
            R"(@.zero_div_err = constant [24 x i8] c"Error division by zero\0A\00")"
    };
    for (auto& glob: globals) {
        codeBuffer.emitGlobal(glob);
    }
    codeBuffer.emitLabel("define void @printi(i32) {");
    codeBuffer.emit("call i32 (i8*, ...) @printf(i8* getelementptr([4 x i8], [4 x i8]* @.int_specifier, i32 0, i32 0), i32 %0)");
    codeBuffer.emit("ret void");
    codeBuffer.emitLabel("}");
    codeBuffer.emitLabel("define void @print(i8*) {");
    codeBuffer.emit("call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @.str_specifier, i32 0, i32 0), i8* %0)");
    codeBuffer.emit("ret void");
    codeBuffer.emitLabel("}");

}
