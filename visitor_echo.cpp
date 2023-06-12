//
// Created by Snir Bachar on 06/06/2023.
//
#include <iostream>
#include "visitor.hpp"

void EchoVisitor::visitFormalDecl(FormalDeclNode &node) {
	std::cout << "FormalDecl" << std::endl;
}

void EchoVisitor::visitFormalList(FormalListNode &node) {
	std::cout << "FormalList" << std::endl;
}

void EchoVisitor::visitFormals(FormalsNode &node) {
	std::cout << "Formals" << std::endl;
}

void EchoVisitor::visitFuncDecl(FuncDeclNode &node) {
	std::cout << "FuncDecl" << std::endl;
}

void EchoVisitor::visitFuncs(FuncsNode &node) {
	std::cout << "Funcs" << std::endl;
}

void EchoVisitor::visitOverride(OverrideNode &node) {
	std::cout << "Override: " << node.getOverride() << std::endl;
}

void EchoVisitor::visitExpr(ExprNode &node) {
	std::cout << "Expr" << std::endl;
}

void EchoVisitor::visitId(IdNode &node) {
	std::cout << "Id" << std::endl;
}

void EchoVisitor::visitExprList(ExprListNode &node) {
	std::cout << "ExprList" << std::endl;
}

void EchoVisitor::visitType(TypeNode &node) {
	std::cout << "Type: " << node.getType() << std::endl;
}

void EchoVisitor::visitRetType(RetTypeNode &node) {
	std::cout << "RetType" << std::endl;
}

void EchoVisitor::visitStatement(StatementNode &node) {
	std::cout << "Statement" << std::endl;
}

void EchoVisitor::visitStatements(StatementsNode &node) {
	std::cout << "Statements" << std::endl;
}

void EchoVisitor::visitCall(CallNode &node) {
	std::cout << "Call: " << node.getId() << std::endl;
}

void EchoVisitor::visitCallStatement(CallStatementNode &node) {
	std::cout << "CallStatement" << std::endl;
}

void EchoVisitor::visitNum(NumNode &node) {
	std::cout << "Num: " << node.getNum() << std::endl;
}

void EchoVisitor::visitString(StringNode &node) {
	std::cout << "String: " << node.getStr() << std::endl;
}

void EchoVisitor::visitBool(BoolNode &node) {
	std::cout << "Bool" << std::endl;
}

void EchoVisitor::visitNot(NotNode &node) {
	std::cout << "Not" << std::endl;
}

void EchoVisitor::visitCallExpr(CallExprNode &node) {
	std::cout << "CallExpr" << std::endl;
}

void EchoVisitor::visitAnd(AndNode &node) {
	std::cout << "And" << std::endl;
}

void EchoVisitor::visitOr(OrNode &node) {
	std::cout << "Or" << std::endl;
}

void EchoVisitor::visitRelOp(RelOpNode &node) {
	std::cout << "RelOp" << std::endl;
}

void EchoVisitor::visitCast(CastNode &node) {
	std::cout << "Cast" << std::endl;
}

void EchoVisitor::visitReturn(ReturnNode &node) {
	std::cout << "Return" << std::endl;
}

void EchoVisitor::visitIf(IfNode &node) {
	std::cout << "If" << std::endl;
}

void EchoVisitor::visitWhile(WhileNode &node) {
	std::cout << "While" << std::endl;
}

void EchoVisitor::visitBreak(BreakNode &node) {
	std::cout << "Break" << std::endl;
}

void EchoVisitor::visitContinue(ContinueNode &node) {
	std::cout << "Continue" << std::endl;
}

void EchoVisitor::visitBinOp(BinOpNode &node) {
	std::cout << "BinOp" << std::endl;
}

void EchoVisitor::visitProgram(ProgramNode &node) {
	std::cout << "Program" << std::endl;
}
void EchoVisitor::visitBoolExpr(BoolExprNode &node) {
	std::cout << "BoolExpr" << std::endl;
}

void EchoVisitor::visitVarDecl(VarDeclNode &node) {
	std::cout << "VarDecl: " << node.getId() << std::endl;
}

void EchoVisitor::visitBlock(BlockNode &node) {
	std::cout << "Block" << std::endl;
}

void EchoVisitor::visitAssign(AssignNode &node) {
	std::cout << "Assign" << std::endl;
}


void EchoVisitor::visitNMarker(NMarkerNode &node) {
	std::cout << "NMarker" << std::endl;
}

void EchoVisitor::visitMMarker(MMarkerNode &node) {
	std::cout << "MMarker" << std::endl;
}

void EchoVisitor::visitIfLabel(IfLabelNode &node) {
	std::cout << "IfLabel" << std::endl;
}
void EchoVisitor::visitElseLabel(ElseLabelNode &node) {
	std::cout << "ElseLabel" << std::endl;
}
void EchoVisitor::visitWhileLabel(WhileLabelNode &node) {
	std::cout << "WhileLabel" << std::endl;
}