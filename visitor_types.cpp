//
// Created by Snir Bachar on 06/06/2023.
//
#include "visitor.hpp"

void TypesVisitor::visitId(IdNode &node) {
	auto id = symbolTable.firstIdSymbol(node.getId());
	if(id) {
		node.expr_type = id->type.getType();
	}
}

void TypesVisitor::visitNum(NumNode &node) {
	if(node.isByte()) {
		node.expr_type = Types::BYTE_T;
	} else {
		node.expr_type = Types::INT_T;
	}
}

void TypesVisitor::visitString(StringNode &node) {
	node.expr_type = Types::STRING_T;
}

void TypesVisitor::visitBool(BoolNode &node) {
	node.expr_type = Types::BOOL_T;
}

void TypesVisitor::visitNot(NotNode &node) {
	node.expr_type = Types::BOOL_T;
}

void TypesVisitor::visitCallExpr(CallExprNode &node) {
	FuncSymbol *func = symbolTable.getFuncSymbol(node.getId(), node.getArgs());
	if(func) {
		node.expr_type = func->type.getType();
	}
}

void TypesVisitor::visitAnd(AndNode &node) {
	node.expr_type = Types::BOOL_T;
}

void TypesVisitor::visitOr(OrNode &node) {
	node.expr_type = Types::BOOL_T;
}

void TypesVisitor::visitRelOp(RelOpNode &node) {
	node.expr_type = Types::BOOL_T;
}

void TypesVisitor::visitCast(CastNode &node) {
	node.expr_type = node.getType()->getType();
}

Types evaluate_type(Types t1, Types t2) {
	if (t1 == Types::INT_T || t2 == Types::INT_T) {
		return Types::INT_T;
	} else if (t1 == Types::BYTE_T && t2 == Types::BYTE_T) {
		return Types::BYTE_T;
	} else {
		throw std::runtime_error("Type mismatch");
	}
}

void TypesVisitor::visitBinOp(BinOpNode &node) {
	node.expr_type = evaluate_type(node.getLeft()->expr_type, node.getRight()->expr_type);
}

void TypesVisitor::visitBoolExpr(BoolExprNode &node) {
	node.expr_type = Types::BOOL_T;
}
