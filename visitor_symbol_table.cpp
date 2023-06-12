//
// Created by Snir Bachar on 06/06/2023.
//
#include "visitor.hpp"

void SymbolTableVisitor::visitRetType(RetTypeNode &node) {
	symbolTable.setCurrentRetType(*node.getType());
}

void SymbolTableVisitor::visitFuncId(FuncIdNode &node) {
	symbolTable.setCurrentFuncId(node.getId());
}

void SymbolTableVisitor::visitFormals(FormalsNode &node) {
	symbolTable.setCurrentFormals(node);
}

void SymbolTableVisitor::visitOverride(OverrideNode &node) {
	symbolTable.setCurrentOverride(node.getOverride());
}

void SymbolTableVisitor::visitVarDecl(VarDeclNode &node) {
	symbolTable.addIdSymbol(node.getId(), *node.getType());
}

void SymbolTableVisitor::visitProgram(ProgramNode &node) {
	symbolTable.endScope();
}


