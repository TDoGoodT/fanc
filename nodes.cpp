//
// Created by Snir Bachar on 05/06/2023.
//

#include "nodes.hpp"
#include "visitor.hpp"

void FormalDeclNode::accept(ParserVisitor &visitor) {
	visitor.visitFormalDecl(*this);
}

void FormalListNode::accept(ParserVisitor &visitor) {
	visitor.visitFormalList(*this);
}

void FormalsNode::accept(ParserVisitor &visitor) {
	visitor.visitFormals(*this);
}

void FuncDeclNode::accept(ParserVisitor &visitor) {
	visitor.visitFuncDecl(*this);
}

void FuncIdNode::accept(ParserVisitor &visitor) {
	visitor.visitFuncId(*this);
}

void FuncsNode::accept(ParserVisitor &visitor) {
	visitor.visitFuncs(*this);
}

void OverrideNode::accept(ParserVisitor &visitor) {
	visitor.visitOverride(*this);
}

void ExprNode::accept(ParserVisitor &visitor) {
	visitor.visitExpr(*this);
}

void IdNode::accept(ParserVisitor &visitor) {
	visitor.visitId(*this);
}

void ExprListNode::accept(ParserVisitor &visitor) {
	visitor.visitExprList(*this);
}

void TypeNode::accept(ParserVisitor &visitor) {
	visitor.visitType(*this);
}

void RetTypeNode::accept(ParserVisitor &visitor) {
	visitor.visitRetType(*this);
}

void StatementNode::accept(ParserVisitor &visitor) {
	visitor.visitStatement(*this);
}

void StatementsNode::accept(ParserVisitor &visitor) {
	visitor.visitStatements(*this);
}

void CallNode::accept(ParserVisitor &visitor) {
	visitor.visitCall(*this);
}

void CallStatementNode::accept(ParserVisitor &visitor) {
	visitor.visitCallStatement(*this);
}

void NumNode::accept(ParserVisitor &visitor) {
	visitor.visitNum(*this);
}

void StringNode::accept(ParserVisitor &visitor) {
	visitor.visitString(*this);
}

void BoolNode::accept(ParserVisitor &visitor) {
	visitor.visitBool(*this);
}

void NotNode::accept(ParserVisitor &visitor) {
	visitor.visitNot(*this);
}

void CallExprNode::accept(ParserVisitor &visitor) {
	visitor.visitCallExpr(*this);
}

void AndNode::accept(ParserVisitor &visitor) {
	visitor.visitAnd(*this);
}

void OrNode::accept(ParserVisitor &visitor) {
	visitor.visitOr(*this);
}

void RelOpNode::accept(ParserVisitor &visitor) {
	visitor.visitRelOp(*this);
}

void CastNode::accept(ParserVisitor &visitor) {
	visitor.visitCast(*this);
}

void ReturnNode::accept(ParserVisitor &visitor) {
	visitor.visitReturn(*this);
}

void IfNode::accept(ParserVisitor &visitor) {
	visitor.visitIf(*this);
}

void WhileNode::accept(ParserVisitor &visitor) {
	visitor.visitWhile(*this);
}
void BoolExprNode::accept(ParserVisitor &visitor) {
	visitor.visitBoolExpr(*this);
}

void BreakNode::accept(ParserVisitor &visitor) {
	visitor.visitBreak(*this);
}

void ContinueNode::accept(ParserVisitor &visitor) {
	visitor.visitContinue(*this);
}

void BinOpNode::accept(ParserVisitor &visitor) {
	visitor.visitBinOp(*this);
}

void ProgramNode::accept(ParserVisitor &visitor) {
	visitor.visitProgram(*this);
}

void VarDeclNode::accept(ParserVisitor &visitor) {
	visitor.visitVarDecl(*this);
}

void BlockNode::accept(ParserVisitor &visitor) {
	visitor.visitBlock(*this);
}

void AssignNode::accept(ParserVisitor &visitor) {
	visitor.visitAssign(*this);
}

void MMarkerNode::accept(ParserVisitor &visitor) {
	visitor.visitMMarker(*this);
}

void NMarkerNode::accept(ParserVisitor &visitor) {
	visitor.visitNMarker(*this);
}

void IfLabelNode::accept(class ParserVisitor &visitor) {
	visitor.visitIfLabel(*this);
}
void ElseLabelNode::accept(class ParserVisitor &visitor) {
	visitor.visitElseLabel(*this);
}
void WhileLabelNode::accept(class ParserVisitor &visitor) {
	visitor.visitWhileLabel(*this);
}