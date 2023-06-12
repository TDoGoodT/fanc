//
// Created by Snir Bachar on 06/06/2023.
//

#ifndef FANC_ANALYZER_VISITOR_H
#define FANC_ANALYZER_VISITOR_H

#include "nodes.hpp"
#include "symbol_table.hpp"
#include "bp.hpp"

class 	ParserVisitor {
public:
	virtual void visitFormalDecl(FormalDeclNode &node) {};

	virtual void visitFormalList(FormalListNode &node) {};

	virtual void visitFormals(FormalsNode &node) {};

	virtual void visitFuncDecl(FuncDeclNode &node) {};

	virtual void visitFuncs(FuncsNode &node) {};

	virtual void visitOverride(OverrideNode &node) {};

	virtual void visitExpr(ExprNode &node) {};

	virtual void visitId(IdNode &node) {};

	virtual void visitExprList(ExprListNode &node) {};

	virtual void visitType(TypeNode &node) {};

	virtual void visitRetType(RetTypeNode &node) {};

	virtual void visitStatement(StatementNode &node) {};

	virtual void visitStatements(StatementsNode &node) {};

	virtual void visitCall(CallNode &node) {};

	virtual void visitCallStatement(CallStatementNode &node) {};

	virtual void visitNum(NumNode &node) {};

	virtual void visitString(StringNode &node) {};

	virtual void visitBool(BoolNode &node) {};

	virtual void visitNot(NotNode &node) {};

	virtual void visitCallExpr(CallExprNode &node) {};

	virtual void visitAnd(AndNode &node) {};

	virtual void visitOr(OrNode &node) {};

	virtual void visitRelOp(RelOpNode &node) {};

	virtual void visitCast(CastNode &node) {};

	virtual void visitReturn(ReturnNode &node) {};

	virtual void visitIf(IfNode &node) {};

	virtual void visitWhile(WhileNode &node) {};

	virtual void visitBoolExpr(BoolExprNode &node) {};

	virtual void visitBreak(BreakNode &node) {};

	virtual void visitContinue(ContinueNode &node) {};

	virtual void visitBinOp(BinOpNode &node) {};

	virtual void visitProgram(ProgramNode &node) {};

	virtual void visitVarDecl(VarDeclNode &node) {};

	virtual void visitBlock(BlockNode &node) {};

	virtual void visitAssign(AssignNode &node) {};

	virtual void visitFuncId(FuncIdNode &node) {};

	virtual void visitMMarker(MMarkerNode &node) {};

	virtual void visitNMarker(NMarkerNode &node) {};
	virtual void visitIfLabel(IfLabelNode &node) {};
	virtual void visitElseLabel(ElseLabelNode &node) {};
	virtual void visitWhileLabel(WhileLabelNode &node) {};
};


class SymbolTableVisitor : public ParserVisitor {
private:
	SymbolTable &symbolTable;
public:
	explicit SymbolTableVisitor(SymbolTable &symbolTable) : symbolTable(symbolTable) {}

	void visitProgram(ProgramNode &node) override;

	void visitVarDecl(VarDeclNode &node) override;

	void visitFuncId(FuncIdNode &node) override;

	void visitFormals(FormalsNode &node) override;

	void visitRetType(RetTypeNode &node) override;

	void visitOverride(OverrideNode &node) override;
};

class ValidationVisitor : public ParserVisitor {
private:
	SymbolTable &symbolTable;
	WhileContext &context;
public:
	explicit ValidationVisitor(SymbolTable &symbolTable, WhileContext &context) : symbolTable(symbolTable),
																				  context(context) {}

	void visitFuncId(FuncIdNode &node) override;
	void visitFormals(FormalsNode &node) override;

	void visitBoolExpr(BoolExprNode &node) override;

	void visitFuncDecl(FuncDeclNode &node) override;

	void visitId(IdNode &node) override;

	void visitCall(CallNode &node) override;

	void visitNum(NumNode &node) override;

	void visitNot(NotNode &node) override;

	void visitAnd(AndNode &node) override;

	void visitOr(OrNode &node) override;

	void visitRelOp(RelOpNode &node) override;

	void visitCast(CastNode &node) override;

	void visitReturn(ReturnNode &node) override;

	void visitIf(IfNode &node) override;

	void visitWhile(WhileNode &node) override;

	void visitBreak(BreakNode &node) override;

	void visitContinue(ContinueNode &node) override;

	void visitBinOp(BinOpNode &node) override;

	void visitProgram(ProgramNode &node) override;

	void visitVarDecl(VarDeclNode &node) override;

	void visitAssign(AssignNode &node) override;
};


class EchoVisitor : public ParserVisitor {
public:
	EchoVisitor() = default;

	void visitBoolExpr(BoolExprNode &node) override;

	void visitFormalDecl(FormalDeclNode &node) override;

	void visitFormalList(FormalListNode &node) override;

	void visitFormals(FormalsNode &node) override;

	void visitFuncDecl(FuncDeclNode &node) override;

	void visitFuncs(FuncsNode &node) override;

	void visitOverride(OverrideNode &node) override;

	void visitExpr(ExprNode &node) override;

	void visitId(IdNode &node) override;

	void visitExprList(ExprListNode &node) override;

	void visitType(TypeNode &node) override;

	void visitRetType(RetTypeNode &node) override;

	void visitStatement(StatementNode &node) override;

	void visitStatements(StatementsNode &node) override;

	void visitCall(CallNode &node) override;

	void visitCallStatement(CallStatementNode &node) override;

	void visitNum(NumNode &node) override;

	void visitString(StringNode &node) override;

	void visitBool(BoolNode &node) override;

	void visitNot(NotNode &node) override;

	void visitCallExpr(CallExprNode &node) override;

	void visitAnd(AndNode &node) override;

	void visitOr(OrNode &node) override;

	void visitRelOp(RelOpNode &node) override;

	void visitCast(CastNode &node) override;

	void visitReturn(ReturnNode &node) override;

	void visitIf(IfNode &node) override;

	void visitWhile(WhileNode &node) override;

	void visitBreak(BreakNode &node) override;

	void visitContinue(ContinueNode &node) override;

	void visitBinOp(BinOpNode &node) override;

	void visitProgram(ProgramNode &node) override;

	void visitVarDecl(VarDeclNode &node) override;

	void visitBlock(BlockNode &node) override;

	void visitAssign(AssignNode &node) override;

	void visitNMarker(NMarkerNode &node) override;
	void visitMMarker(MMarkerNode &node) override;

	void visitIfLabel(IfLabelNode &node) override;

	void visitElseLabel(ElseLabelNode &node) override;

	void visitWhileLabel(WhileLabelNode &node) override;
};

class CodeGenVisitor : public ParserVisitor {
	SymbolTable &symbol_table;
	CodeBuffer &buffer;
public:
	CodeGenVisitor(SymbolTable &symbolTable, CodeBuffer &buffer) : symbol_table(symbolTable),
																			   buffer(buffer) {}
	void visitStatement(StatementNode &node) override;
	void visitStatements(StatementsNode &node) override;

	void visitBoolExpr(BoolExprNode &node) override;

	void visitFormalDecl(FormalDeclNode &node) override;

	void visitFormalList(FormalListNode &node) override;

	void visitFormals(FormalsNode &node) override;

	void visitFuncDecl(FuncDeclNode &node) override;

	void visitFuncs(FuncsNode &node) override;

	void visitExpr(ExprNode &node) override;

	void visitId(IdNode &node) override;

	void visitExprList(ExprListNode &node) override;

	void visitCall(CallNode &node) override;

	void visitCallStatement(CallStatementNode &node) override;

	void visitNum(NumNode &node) override;

	void visitString(StringNode &node) override;

	void visitBool(BoolNode &node) override;

	void visitNot(NotNode &node) override;

	void visitCallExpr(CallExprNode &node) override;

	void visitAnd(AndNode &node) override;

	void visitOr(OrNode &node) override;

	void visitRelOp(RelOpNode &node) override;

	void visitCast(CastNode &node) override;

	void visitReturn(ReturnNode &node) override;

	void visitIf(IfNode &node) override;

	void visitWhile(WhileNode &node) override;

	void visitBreak(BreakNode &node) override;

	void visitContinue(ContinueNode &node) override;

	void visitBinOp(BinOpNode &node) override;

	void visitProgram(ProgramNode &node) override;

	void visitVarDecl(VarDeclNode &node) override;

	void visitBlock(BlockNode &node) override;

	void visitAssign(AssignNode &node) override;

	void visitMMarker(MMarkerNode &node) override;

	void visitNMarker(NMarkerNode &node) override;
};


class TypesVisitor : public ParserVisitor {
private:
	SymbolTable &symbolTable;
public:
	explicit TypesVisitor(SymbolTable &symbolTable) : symbolTable(symbolTable) {}

	void visitId(IdNode &node) override;

	void visitBoolExpr(BoolExprNode &node) override;

	void visitNum(NumNode &node) override;

	void visitString(StringNode &node) override;

	void visitBool(BoolNode &node) override;

	void visitNot(NotNode &node) override;

	void visitCallExpr(CallExprNode &node) override;

	void visitAnd(AndNode &node) override;

	void visitOr(OrNode &node) override;

	void visitRelOp(RelOpNode &node) override;

	void visitCast(CastNode &node) override;

	void visitBinOp(BinOpNode &node) override;
};

class ScopeVisitor : public ParserVisitor {
private:
	SymbolTable &symbolTable;
	WhileContext &context;
public:
	explicit ScopeVisitor(SymbolTable &symbolTable, WhileContext &context) : symbolTable(symbolTable),
																			 context(context) {}

	void visitIfLabel(IfLabelNode &node) override;

	void visitWhileLabel(WhileLabelNode &node) override;

	void visitElseLabel(ElseLabelNode &node) override;
	
	void visitFuncDecl(FuncDeclNode &node) override;

	void visitBlock(BlockNode &node) override;

	void visitWhile(WhileNode &node) override;

	void visitIf(IfNode &node) override;

};

#endif //FANC_ANALYZER_VISITOR_H
