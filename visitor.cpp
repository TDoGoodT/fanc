//
// Created by snir on 1/25/23.
//

#include <cassert>
#include <iostream>
#include "visitor.hpp"

CodeBuffer &codeBuffer = CodeBuffer::instance();


string to_string(Type *pType) {
    switch (pType->typeCase) {
        case Type::INT_:
            return " i32 ";
        case Type::BOOL_:
            return " i1 ";
        case Type::BYTE_:
            return " i8 ";
        case Type::STRING_:
        case Type::VOID_:
            assert(false);
    }
    exit(0);
}

string getLlvmBinOp(BinopCase binopCase) {
    switch (binopCase) {
        case _PLUS_:
            return " add ";
        case _MINUS_:
            return " sub ";
        case _MULT_:
            return " mul ";
        case _DIV_:
            return " sdiv ";
    }
    exit(0);
}

void Visitor::visit(Binop *element) {
    string opStr = getLlvmBinOp(element->binopCase);
    string rStr = getValuePlace(element->rExp);
    string lStr = getValuePlace(element->lExp);
    element->place = codeBuffer.newTemp();
    if (element->binopCase == _DIV_) {
        string zeroDivVar = codeBuffer.newTemp();
        codeBuffer.emit(zeroDivVar + " = icmp eq i32 " + rStr + ", 0");
        codeBuffer.emit("br i1 " + zeroDivVar + " , label @ , label @");
        codeBuffer.emit(
                "call i32 (i8*, ...) @printf(i8* getelementptr([24 x i8], [24 x i8]* @.zero_div_err, i32 0, i32 0))");
        codeBuffer.emit("call void @exit(i32 0)");
        codeBuffer.emit("br label @");
    }
    if (element->type->typeCase != Type::BYTE_) {
        codeBuffer.emit("%"+element->place + " = " + opStr + to_string(element->type) + "%"+lStr + ", %" + rStr);
        return;
    }
    string maskedVar = codeBuffer.newTemp();
    if (element->rExp->type->typeCase == Type::INT_) {
        codeBuffer.emit(maskedVar + " = and i32 " + rStr + ", 255");
        rStr = maskedVar;
    }
}

void Visitor::visit(Exp *element) {

}

void Visitor::visit(Or *element) {
    codeBuffer.bpatch(element->lExp->falseList, element->orMarker->label);
    element->trueList = CodeBuffer::merge(element->lExp->trueList, element->rExp->trueList);
    element->falseList = element->rExp->falseList;

}

void Visitor::visit(And *element) {
    codeBuffer.bpatch(element->lExp->trueList, element->andMarker->label);
    element->trueList = element->rExp->trueList;
    element->falseList = CodeBuffer::merge(element->lExp->falseList, element->rExp->falseList);
}

void Visitor::visit(Number *element) {

}

void Visitor::visit(Int *element) {
    element->place = codeBuffer.newTemp();
    assert(element->type->typeCase == Type::INT_);
    codeBuffer.emit("%" + element->place + " = "
                    + getLlvmBinOp(BinopCase::_PLUS_) + to_string(element->type) + " " + to_string(*element->value) +
                    ", 0; visited Int " + to_string(*element->value));
}

void Visitor::visit(Byte *element) {
    element->place = codeBuffer.newTemp();
    assert(element->type->typeCase == Type::BYTE_);
    codeBuffer.emit("%" + element->place + " = " + getLlvmBinOp(BinopCase::_PLUS_) + to_string(element->type) + " " +
                    to_string(*element->value) + ", 0; visited Byte " + to_string(*element->value));
}

void Visitor::visit(Declaration *element) {
    element->id->place = codeBuffer.newVar();
    codeBuffer.emit("%" + element->id->place + " = alloca i32; visited Declaration " +
                    element->type->to_string() + " " + element->id->id);
}

void Visitor::visit(Assignment *element) {
    if (element->value->type->typeCase == Type::BOOL_) {
        handleBoolAssignment(element->value);
    }
    element->id->place = codeBuffer.newVar();
    codeBuffer.emit("%" + element->id->place + " = alloca i32; visited Assignment " +
                    element->type->to_string() + " " + element->id->id + " = %" + element->value->place);
    string valuePlace = getValuePlace(element->value);
    codeBuffer.emit(
            "store i32 %" + valuePlace + ", i32 * %" +
            element->id->place);
}

void Visitor::handleBoolAssignment(struct Exp *value) {
    string trueLabel = codeBuffer.genLabel();
    int trueLine = codeBuffer.emit("br label @; true ");
    string falseLabel = codeBuffer.genLabel();
    int falseLine = codeBuffer.emit("br label @; false ");
    string nextLabel = codeBuffer.genLabel();
    codeBuffer.emit("%" + value->place + " = phi i1 [1,%" + trueLabel + "], [0,%" + falseLabel + "]");

    codeBuffer.bpatch(value->trueList, trueLabel);
    codeBuffer.bpatch(value->falseList, falseLabel);
    codeBuffer.bpatch(CodeBuffer::makelist({trueLine, FIRST}), nextLabel);
    codeBuffer.bpatch(CodeBuffer::makelist({falseLine, FIRST}), nextLabel);
}

void Visitor::visit(LateAssignment *element) {
    if (element->value->type->typeCase == Type::BOOL_) {
        handleBoolAssignment(element->value);
    }
    string valuePlace = getValuePlace(element->value);
    codeBuffer.emit("store " + to_string(element->value->type) + "%" + valuePlace + ", " +
                    to_string(element->id->type) + "* %" + element->id->place);
}

string Visitor::getValuePlace(const Exp *value) {
    string valuePlace = value->place;
    if(value->place.empty()) return to_string(*value->value);
    if(value->place.find('t') == string::npos) {
        valuePlace = codeBuffer.newTemp();
        codeBuffer.emit("%" + valuePlace + " = load i32, i32* %" + value->place);
    }
    return valuePlace;
};

void Visitor::visit(FunctionCall *element) {

}

void Visitor::visit(Return *element) {

}

void Visitor::visit(If_pattern *element) {

}

void Visitor::visit(While *element) {

}

void Visitor::visit(Statements *element) {

}

void Visitor::visit(Statement *element) {

}

void Visitor::visit(Not *element) {
    element->trueList = element->exp->falseList;
    element->falseList = element->exp->trueList;
}


void Visitor::visit(Relop *element) {
    string temp = codeBuffer.newTemp();
    codeBuffer.emit(
            "%" + temp + " = icmp " + getLlvmRelop(element) + " " + to_string(element->type) + " %" +
            element->lExp->place +
            ", %" + element->rExp->place);
    codeBuffer.emit("br i1 %" + temp + ", label @, label @");
    element->trueList = CodeBuffer::makelist({codeBuffer.getLineNumber(), FIRST});
    element->falseList = CodeBuffer::makelist({codeBuffer.getLineNumber(), SECOND});
}

void Visitor::visit(AndMarker *element) {
    element->label = codeBuffer.genLabel();
}

void Visitor::visit(OrMarker *element) {
    element->label = codeBuffer.genLabel();
}

void Visitor::visit(EndMarker *element) {
    codeBuffer.printCodeBuffer();
}

string Visitor::getLlvmRelop(struct Relop *relop) {
    switch (relop->relopCase) {
        case GT_:
            return " sgt ";
        case GE_:
            return " sge ";
        case LT_:
            return " slt ";
        case LE_:
            return " sle ";
        case EQ_:
            return " eq ";
        case NE_:
            return " ne ";
    }
    exit(-1);
}


void Visitor::visit(Bool *element) {
    assert(element->type->typeCase == Type::BOOL_);
    codeBuffer.emit("br label @; visited Bool " + to_string(*element->value));
    if (*element->value) element->trueList = CodeBuffer::makelist({codeBuffer.getLineNumber(), FIRST});
    else element->falseList = CodeBuffer::makelist({codeBuffer.getLineNumber(), FIRST});
}

void Visitor::visit(RetType *element) {

}

void Visitor::visit(Program *element) {

}

void Visitor::visit(Funcs *element) {

}

void Visitor::visit(FuncDecl *element) {

}

void Visitor::visit(Formals *element) {

}

void Visitor::visit(FormalsList *element) {

}

void Visitor::visit(FormalDecl *element) {

}

void Visitor::visit(ExpList *element) {

}


void Visitor::visit(Trinari *element) {

}

void Visitor::visit(Call *element) {

}

void Visitor::visit(CallExp *element) {

}

void Visitor::visit(InitMarker *element) {
    string globals[] = {
            "declare i32 @printf(i8*, ...)",
            "declare void @exit(i32)",
            R"(@.int_specifier = constant [4 x i8] c"%d\0A\00")",
            R"(@.str_specifier = constant [4 x i8] c"%s\0A\00")",
            R"(@.zero_div_err = constant [24 x i8] c"Error division by zero\0A\00")"
    };
    for (auto &glob: globals) {
        codeBuffer.emitGlobal(glob);
    }
    codeBuffer.emitLabel("define void @printi(i32) {");
    codeBuffer.emit(
            "call i32 (i8*, ...) @printf(i8* getelementptr([4 x i8], [4 x i8]* @.int_specifier, i32 0, i32 0), i32 %0)");
    codeBuffer.emit("ret void");
    codeBuffer.emitLabel("}");
    codeBuffer.emitLabel("define void @print(i8*) {");
    codeBuffer.emit(
            "call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @.str_specifier, i32 0, i32 0), i8* %0)");
    codeBuffer.emit("ret void");
    codeBuffer.emitLabel("}");
    codeBuffer.emitLabel("program:");

}

void Visitor::visit(struct Cast *element) {

}

void Visitor::visit(struct String *element) {

}

void Visitor::visit(struct Id *element) {

}
