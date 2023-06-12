//
// Created by Snir Bachar on 06/06/2023.
//
#include "visitor.hpp"
void ScopeVisitor::visitFuncDecl(FuncDeclNode &node)
{
	symbolTable.endScope();
}

void ScopeVisitor::visitBlock(BlockNode &node)
{
	symbolTable.endScope();
}

void ScopeVisitor::visitWhile(WhileNode &node)
{
	symbolTable.endScope();
	context.exitWhile();
}
void ScopeVisitor::visitIf(IfNode &node)
{
	symbolTable.endScope();
}

void ScopeVisitor::visitIfLabel(IfLabelNode &node)
{
	symbolTable.newScope();
}

void ScopeVisitor::visitWhileLabel(WhileLabelNode &node)
{
	context.enterWhile();
	symbolTable.newScope();
}

void ScopeVisitor::visitElseLabel(ElseLabelNode &node)
{
	symbolTable.endScope();
	symbolTable.newScope();
}