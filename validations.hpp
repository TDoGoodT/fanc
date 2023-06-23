//
// Created by Snir Bachar on 07/06/2023.
//

#ifndef FANC_ANALYZER_VALIDATIONS_H
#define FANC_ANALYZER_VALIDATIONS_H

#include "nodes.hpp"
#include "symbol_table.hpp"
#include <algorithm>

static inline void validateNotMain(FuncDeclNode &node) {
	if (node.getId()->getId() == "main") {
		output::errorMainOverride(yylineno);
		exit(1);
	}
}

static inline void validateIsNumber(Types type) {
	if (type != Types::INT_T && type != Types::BYTE_T) {
		output::errorMismatch(yylineno);
		exit(1);
	}
}

static inline void validateMainExists(const std::vector<const FuncDeclNode *> &funcs) {
	bool found = false;
	for (auto func: funcs) {
		if (func->getId()->getId() == "main" && func->getFormals()->getFormalDecls() == nullptr &&
			func->getRetType()->getType()->getType() == Types::VOID_T) {
			found = true;
			break;
		}
	}
	if (!found) {
		output::errorMainMissing();
		exit(1);
	}
}

static inline void validateIsBool(Types types) {
	if (types != Types::BOOL_T) {
		output::errorMismatch(yylineno);
		exit(1);
	}
}

static inline void validateCast(Types from, Types to, bool explicit_cast = false) {
	if (!types_match(from, to, explicit_cast)) {
		output::errorMismatch(yylineno);
		exit(1);
	}
}

static inline void validateIdIsDefined(IdNode &node, SymbolTable &symbolTable) {
	if (symbolTable.firstIdSymbol(node.getId()) == nullptr) {
		output::errorUndef(yylineno, node.getId());
		exit(1);
	}
}

static inline void validateIsOverride(FuncDeclNode &node, SymbolTable &symbolTable) {
	if (!symbolTable.isOverride(node.getId()->getId())) {
		output::errorFuncNoOverride(yylineno, node.getId()->getId());
		exit(1);
	}
}

static inline void validateFuncDoesntExist(const std::string &id, SymbolTable &symbolTable) {
	if (symbolTable.getFuncsByName(id).size() > 0) {
		output::errorDef(yylineno, id);
		exit(1);
	}
}
static inline void validateIdDoesntExist(std::string id,const std::vector<FormalDeclNode *> &formals) {
	vector<string> seen_ids;
	for (auto formal: formals) {
		if (std::count(seen_ids.begin(), seen_ids.end(), formal->getId()) > 1) {
			output::errorDef(yylineno, id);
			exit(1);
		}
		seen_ids.push_back(formal->getId());
	}
}
static inline void validateIsntOverride(FuncDeclNode &node, SymbolTable &symbolTable) {
	if (symbolTable.isOverride(node.getId()->getId())) {
		output::errorOverrideWithoutDeclaration(yylineno, node.getId()->getId());
		exit(1);
	}
}

static inline void validatePrototypeMatch(FuncDeclNode &node, FuncSymbol *func) {
	if (func->formals.getFormalDecls()->getFormalDecls().size() !=
		node.getFormals()->getFormalDecls()->getFormalDecls().size()) {
		output::errorPrototypeMismatch(yylineno, node.getId()->getId());
		exit(1);
	}
}

#endif //FANC_ANALYZER_VALIDATIONS_H
