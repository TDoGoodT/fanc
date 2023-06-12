//
// Created by Snir Bachar on 06/06/2023.
//
#include <iostream>
#include <numeric>
#include <string>
#include <vector>
#include "visitor.hpp"


void CodeGenVisitor::visitProgram(ProgramNode &node) {
	string globals[] = {
			"declare i32 @printf(i8*, ...)",
			"declare void @exit(i32)",
			R"(@.int_specifier = constant [4 x i8] c"%d\0A\00")",
			R"(@.str_specifier = constant [4 x i8] c"%s\0A\00")",
			R"(@.zero_div_err = constant [24 x i8] c"Error division by zero\0A\00")"};
	for (auto &glob: globals) {
		buffer.emitGlobal(glob);
	}
	buffer.emitGlobal("define void @printi(i32) {");
	buffer.emitGlobal(
			"\tcall i32 (i8*, ...) @printf(i8* getelementptr([4 x i8], [4 x i8]* @.int_specifier, i32 0, i32 0), i32 %0)");
	buffer.emitGlobal("\tret void");
	buffer.emitGlobal("}");
	buffer.emitGlobal("define void @print(i8*) {");
	buffer.emitGlobal(
			"\tcall i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @.str_specifier, i32 0, i32 0), i8* %0)");
	buffer.emitGlobal("\tret void");
	buffer.emitGlobal("}");

	buffer.printGlobalBuffer();
	buffer.printCodeBuffer();
}

void CodeGenVisitor::visitBinOp(BinOpNode &node) {
	auto op = node.getOp();
	node.place = "%" + buffer.newTemp();
	if (op == "/") {
		string zeroDivVar = buffer.newTemp();
		buffer.emit(zeroDivVar + " = icmp eq i32 " + node.getRight()->place + ", 0");
		buffer.emit("br i1 " + zeroDivVar + " , label @ , label @");
		buffer.emit(
				"call i32 (i8*, ...) @printf(i8* getelementptr([24 x i8], [24 x i8]* @.zero_div_err, i32 0, i32 0))");
		buffer.emit("call void @exit(i32 0)");
		buffer.emit("br label @");
		buffer.emit(node.place + " = sdiv i32 " + node.getLeft()->place + ", " + node.getRight()->place);
	} else if (op == "*") {
		buffer.emit(node.place + " = mul i32 " + node.getLeft()->place + ", " + node.getRight()->place);
	} else if (op == "+") {
		buffer.emit(node.place + " = add i32 " + node.getLeft()->place + ", " + node.getRight()->place);
	} else if (op == "-") {
		buffer.emit(node.place + " = sub i32 " + node.getLeft()->place + ", " + node.getRight()->place);
	}

}

void CodeGenVisitor::visitVarDecl(VarDeclNode &node) {
	buffer.emit("%" + symbol_table.getIdSymbols(node.getId()).back()->label + " = alloca i32");
	if (node.getExpr() == nullptr) {
		buffer.emit("store i32 0, i32* %" + symbol_table.getIdSymbols(node.getId()).back()->label);
		return;
	}
	buffer.emit(
			"store i32 %" + node.getExpr()->place + ", i32* %" + symbol_table.getIdSymbols(node.getId()).back()->label);
}

void CodeGenVisitor::visitAssign(AssignNode &node) {
	buffer.emit(
			"store i32 %" + node.getExpr()->place + ", i32* %" + symbol_table.getIdSymbols(node.getId()).back()->label);
}

void CodeGenVisitor::visitId(IdNode &node) {
	node.place = buffer.newTemp();
	buffer.emit("%" + node.place + " = load i32, i32* %" + symbol_table.getIdSymbols(node.getId()).back()->label);
}

void CodeGenVisitor::visitNum(NumNode &node) {
	node.place = buffer.newTemp();
	buffer.emit("%" + node.place + " = add i32 0, " + std::to_string(node.getNum()));
}

void CodeGenVisitor::visitNMarker(NMarkerNode &node) {
	node.next_list.emplace_back(buffer.labelCounter, BranchLabelIndex::FIRST);
	buffer.genLabel();
	buffer.emit("br label @");
}

void CodeGenVisitor::visitMMarker(MMarkerNode &node) {
	node.label = buffer.genLabel();
}

void CodeGenVisitor::visitIf(IfNode &node) {
	buffer.bpatch(node.getCondition()->true_list, node.getMMarker()->label);
	if(node.getElseBlock() != nullptr) {
		buffer.bpatch(node.getCondition()->false_list, node.getMMarker2()->label);
		auto temp = CodeBuffer::merge(node.getIfBlock()->next_list, node.getNMarker()->next_list);
		node.next_list = CodeBuffer::merge(temp, node.getElseBlock()->next_list);
		return;
	}
	node.next_list = CodeBuffer::merge(node.getCondition()->false_list, node.getIfBlock()->next_list);
}

void CodeGenVisitor::visitWhile(WhileNode &node) {
	buffer.bpatch(node.getBlock()->next_list, node.getMMarker1()->label);
	buffer.bpatch(node.getCondition()->true_list, node.getMMarker2()->label);
	node.next_list = node.getCondition()->false_list;
	buffer.emit("br label %" + node.getMMarker1()->label);
}

void CodeGenVisitor::visitStatements(StatementsNode &node) {
	if(node.getStatements() != nullptr) {
		buffer.bpatch(node.getStatements()->next_list, node.getMMarker()->label);
	}
	node.next_list = node.getStatement()->next_list;
}
void CodeGenVisitor::visitBlock(BlockNode &node) {
	node.next_list = node.getStatements()->next_list;
}

void CodeGenVisitor::visitString(StringNode &node) {
	auto &str = node.getStr();
	node.place = "str_" + buffer.newTemp();
	buffer.emitGlobal("@." + node.place +" = constant ["+ to_string(str.size()) + " x i8] c\"" + str +R"(\00")");
}

void CodeGenVisitor::visitBool(BoolNode &node) {
	auto index = buffer.emit("br label @");
	if (node.getVal()) node.true_list = CodeBuffer::makelist({index, BranchLabelIndex::FIRST});
	else node.false_list = CodeBuffer::makelist({index, BranchLabelIndex::FIRST});
}

void CodeGenVisitor::visitNot(NotNode &node) {
	node.false_list = node.getExpr()->true_list;
	node.true_list = node.getExpr()->false_list;
}

void CodeGenVisitor::visitAnd(AndNode &node) {
	buffer.bpatch(node.getLeft()->true_list, node.getMMarker()->label);
	node.true_list = node.getRight()->true_list;
	node.false_list = CodeBuffer::merge(node.getLeft()->false_list, node.getRight()->false_list);
}

void CodeGenVisitor::visitOr(OrNode &node) {
	buffer.bpatch(node.getLeft()->false_list, node.getMMarker()->label);
	node.true_list = CodeBuffer::merge(node.getLeft()->true_list, node.getRight()->true_list);
	node.false_list = node.getRight()->false_list;
}

void CodeGenVisitor::visitRelOp(RelOpNode &node) {
	auto tmp = buffer.newTemp();
	buffer.emit("%" + tmp + " = icmp " + node.getLlvmOp() + " i32 %" + node.getLeft()->place + ", %" + node.getRight()->place);
	auto index = buffer.emit("br i1 %" + tmp + ", label @, label @");
	node.true_list = CodeBuffer::makelist({index, BranchLabelIndex::FIRST});
	node.false_list = CodeBuffer::makelist({index, BranchLabelIndex::SECOND});
}


void CodeGenVisitor::visitBoolExpr(BoolExprNode &node) {
	node.false_list = node.getCondition()->false_list;
	node.true_list = node.getCondition()->true_list;
}
















void CodeGenVisitor::visitCallExpr(CallExprNode &node) {
	buffer.emit("CallExpr");
}

void CodeGenVisitor::visitCast(CastNode &node) {
	buffer.emit("Cast");
}

void CodeGenVisitor::visitReturn(ReturnNode &node) {
	buffer.emit("Return");
}


void CodeGenVisitor::visitBreak(BreakNode &node) {
	buffer.emit("Break");
}

void CodeGenVisitor::visitContinue(ContinueNode &node) {
	buffer.emit("Continue");
}



void CodeGenVisitor::visitStatement(StatementNode &node) {
	buffer.emit("Statement");
}

void CodeGenVisitor::visitFormalDecl(FormalDeclNode &node) {
	buffer.emit("FormalDecl");
}

void CodeGenVisitor::visitFormalList(FormalListNode &node) {
	buffer.emit("FormalList");
}

void CodeGenVisitor::visitFormals(FormalsNode &node) {
	auto current_function = symbol_table.getAllFuncs().back();
	std::string func_decl_str("define " + current_function.type.getLlvmType() + " @"+ current_function.name + "(");
	auto formal_list_node = current_function.formals.getFormalDecls();
	if(formal_list_node != nullptr) {
		auto formal_vector = formal_list_node->getFormalDecls();
		std::vector<std::string> string_list(formal_vector.size());
		std::transform(formal_vector.begin(), formal_vector.end(), string_list.begin(), [](const FormalDeclNode* formal) { return formal->getType()->getLlvmType(); });
		std::string result = std::accumulate(string_list.begin(), string_list.end(), std::string(), [](const std::string& a, const std::string& b) { return a + ", " + b; });
	}
	func_decl_str += ") {";
	buffer.emitLabel(func_decl_str);
}

void CodeGenVisitor::visitFuncDecl(FuncDeclNode &node) {
	buffer.emitLabel("}");
}

void CodeGenVisitor::visitFuncs(FuncsNode &node) {
	buffer.emit("Funcs");
}

void CodeGenVisitor::visitExpr(ExprNode &node) {
	buffer.emit("Expr");
}

void CodeGenVisitor::visitExprList(ExprListNode &node) {
	buffer.emit("ExprList");
}


void CodeGenVisitor::visitCall(CallNode &node) {
	buffer.emit("Call: ");
}

void CodeGenVisitor::visitCallStatement(CallStatementNode &node) {
	buffer.emit("CallStatement");
}
