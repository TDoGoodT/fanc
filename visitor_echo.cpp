//
// Created by Snir Bachar on 06/06/2023.
//
#include <iostream>
#include "visitor.hpp"

void EchoVisitor::visitFormalDecl(FormalDeclNode &node) {
	buffer.emitComment("FormalDecl");
}

void EchoVisitor::visitFormalList(FormalListNode &node) {
	buffer.emitComment("FormalList");
}

void EchoVisitor::visitFormals(FormalsNode &node) {
	buffer.emitComment("Formals");
}

void EchoVisitor::visitFuncDecl(FuncDeclNode &node) {
	buffer.emitComment("FuncDecl");
}

void EchoVisitor::visitFuncs(FuncsNode &node) {
	buffer.emitComment("Funcs");
}

void EchoVisitor::visitOverride(OverrideNode &node) {
	buffer.emitComment("Override: " + to_string(node.getOverride()));
}

void EchoVisitor::visitExpr(ExprNode &node) {
	buffer.emitComment("Expr");
}

void EchoVisitor::visitId(IdNode &node) {
	buffer.emitComment("Id");
}

void EchoVisitor::visitExprList(ExprListNode &node) {
	buffer.emitComment("ExprList");
}

void EchoVisitor::visitType(TypeNode &node) {
	buffer.emitComment("Type: " + node.getLlvmType());
}

void EchoVisitor::visitRetType(RetTypeNode &node) {
	buffer.emitComment("RetType");
}

void EchoVisitor::visitStatement(StatementNode &node) {
	buffer.emitComment("Statement");
}

void EchoVisitor::visitStatements(StatementsNode &node) {
	buffer.emitComment("Statements");
}

void EchoVisitor::visitCall(CallNode &node) {
	buffer.emitComment("Call: " + node.getId());
}

void EchoVisitor::visitCallStatement(CallStatementNode &node) {
	buffer.emitComment("CallStatement");
}

void EchoVisitor::visitNum(NumNode &node) {
	buffer.emitComment("Num: " + to_string(node.getNum()));
}

void EchoVisitor::visitString(StringNode &node) {
	buffer.emitComment("String: " + node.getStr());
}

void EchoVisitor::visitBool(BoolNode &node) {
	buffer.emitComment("Bool");
}

void EchoVisitor::visitNot(NotNode &node) {
	buffer.emitComment("Not");
}

void EchoVisitor::visitCallExpr(CallExprNode &node) {
	buffer.emitComment("CallExpr");
}

void EchoVisitor::visitAnd(AndNode &node) {
	buffer.emitComment("And");
}

void EchoVisitor::visitOr(OrNode &node) {
	buffer.emitComment("Or");
}

void EchoVisitor::visitRelOp(RelOpNode &node) {
	buffer.emitComment("RelOp");
}

void EchoVisitor::visitCast(CastNode &node) {
	buffer.emitComment("Cast");
}

void EchoVisitor::visitReturn(ReturnNode &node) {
	buffer.emitComment("Return");
}

void EchoVisitor::visitIf(IfNode &node) {
	buffer.emitComment("If");
}

void EchoVisitor::visitWhile(WhileNode &node) {
	buffer.emitComment("While");
}

void EchoVisitor::visitBreak(BreakNode &node) {
	buffer.emitComment("Break");
}

void EchoVisitor::visitContinue(ContinueNode &node) {
	buffer.emitComment("Continue");
}

void EchoVisitor::visitBinOp(BinOpNode &node) {
	buffer.emitComment("BinOp");
}

void EchoVisitor::visitProgram(ProgramNode &node) {
	buffer.emitComment("Program");
}
void EchoVisitor::visitBoolExpr(BoolExprNode &node) {
	buffer.emitComment("BoolExpr");
}

void EchoVisitor::visitVarDecl(VarDeclNode &node) {
	buffer.emitComment("VarDecl: " + node.getId());
}

void EchoVisitor::visitBlock(BlockNode &node) {
	buffer.emitComment("Block");
}

void EchoVisitor::visitAssign(AssignNode &node) {
	buffer.emitComment("Assign");
}


void EchoVisitor::visitNMarker(NMarkerNode &node) {
	buffer.emitComment("NMarker");
}

void EchoVisitor::visitMMarker(MMarkerNode &node) {
	buffer.emitComment("MMarker");
}

void EchoVisitor::visitIfLabel(IfLabelNode &node) {
	buffer.emitComment("IfLabel");
}
void EchoVisitor::visitElseLabel(ElseLabelNode &node) {
	buffer.emitComment("ElseLabel");
}
void EchoVisitor::visitWhileLabel(WhileLabelNode &node) {
	buffer.emitComment("WhileLabel");
}