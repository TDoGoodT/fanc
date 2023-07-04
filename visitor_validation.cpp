//
// Created by Snir Bachar on 06/06/2023.
//
#include "hw3_output.hpp"
#include "visitor.hpp"
#include "validations.hpp"
void ValidationVisitor::visitNot(NotNode &node) {
	validateIsBool(node.getExpr()->expr_type);
}

void ValidationVisitor::visitAnd(AndNode &node) {
	validateIsBool(node.getLeft()->expr_type);
	validateIsBool(node.getRight()->expr_type);
}

void ValidationVisitor::visitOr(OrNode &node) {
	validateIsBool(node.getLeft()->expr_type);
	validateIsBool(node.getRight()->expr_type);
}

void ValidationVisitor::visitRelOp(RelOpNode &node) {
	validateIsNumber(node.getLeft()->expr_type);
	validateIsNumber(node.getRight()->expr_type);
}

void ValidationVisitor::visitNum(NumNode &node) {
	if (node.isByte() && node.getNum() > 255) {
		output::errorByteTooLarge(yylineno, std::to_string(node.getNum()));
		exit(1);
	}
}
void ValidationVisitor::visitBreak(BreakNode &node) {
	if (!context.inWhile()) {
		output::errorUnexpectedBreak(yylineno);
		exit(1);
	}
}

void ValidationVisitor::visitContinue(ContinueNode &node) {
	if (!context.inWhile()) {
		output::errorUnexpectedContinue(yylineno);
		exit(1);
	}
}

void ValidationVisitor::visitReturn(ReturnNode &node) {
	auto expr = node.getExpr();
	if (expr) {
		validateCast(expr->expr_type, symbolTable.getCurrentRetType());
	} else {
		validateCast(Types::VOID_T, symbolTable.getCurrentRetType());
	}
}

void ValidationVisitor::visitCast(CastNode &node) {
	validateCast(node.getExpr()->expr_type, node.getType()->getType(), true);
}

void ValidationVisitor::visitBoolExpr(BoolExprNode &node) {
	validateCast(node.getCondition()->expr_type, Types::BOOL_T);
}

void ValidationVisitor::visitAssign(AssignNode &node) {
	auto idSymbol = symbolTable.firstIdSymbol(node.getId());
	validateCast(node.getExpr()->expr_type, idSymbol->type.getType());
}

void ValidationVisitor::visitVarDecl(VarDeclNode &node) {
	auto scope = symbolTable.getCurrentScope();
	for (auto &idSymbol: scope) {
		if (idSymbol.name == node.getId()) {
			output::errorDef(yylineno, node.getId());
			exit(1);
		}
	}
	auto expr = node.getExpr();
	if (expr) {
		validateCast(expr->expr_type, node.getType()->getType());
	}
}

vector<FuncSymbol *> filter_exact_match(const std::vector<FuncSymbol *> &matches, ExprListNode *args) {
	std::vector<FuncSymbol *> exact_match;
	for (auto &funcSymbol: matches) {
		if(!funcSymbol->formals.getFormalDecls() || !args) {
			if(!funcSymbol->formals.getFormalDecls() && !args) {
				exact_match.push_back(funcSymbol);
			}
			continue;
		}
		auto &formals = funcSymbol->formals.getFormalDecls()->getFormalDecls();
		auto &actuals = args->getExprs();
		if (formals.size() != actuals.size()) {
			continue;
		}
		bool match = true;
		for (int i = 0; i < formals.size(); ++i) {
			auto &formal = formals[i];
			auto &actual = actuals[i];
			if (!types_match(actual->expr_type, formal->getType()->getType())) {
				match = false;
				break;
			}
		}
		if (match) {
			exact_match.push_back(funcSymbol);
		}
	}
	return exact_match;
}

void ValidationVisitor::visitCall(CallNode &node) {
	ExprListNode *args = node.getArgs();
	auto matching_funcs = symbolTable.getFuncsByName(node.getId());
	if (matching_funcs.empty()) {
		output::errorUndefFunc(yylineno, node.getId());
		exit(1);
	}
	std::vector<FuncSymbol *> exact_match = filter_exact_match(matching_funcs, args);
	if (exact_match.size() > 1) {
		output::errorAmbiguousCall(yylineno, node.getId());
		exit(1);
	}
	if (exact_match.empty()) {
		output::errorPrototypeMismatch(yylineno, node.getId());
		exit(1);
	}
}

void ValidationVisitor::visitCallExpr(CallExprNode &node) {
	if (node.expr_type == Types::VOID_T) {
		output::errorMismatch(yylineno);
		exit(1);
	}
}


void ValidationVisitor::visitFormals(FormalsNode &node) {
	auto formals = node.getFormalDecls();
	if(formals) {
		for (auto &formal: formals->getFormalDecls()) {
			validateIdDoesntExist(formal->getId(), formals->getFormalDecls());
		}
	}
}
void ValidationVisitor::visitFuncId(FuncIdNode &node) {
	if (node.getId() == "main" && symbolTable.getCurrentOverride()) {
        output::errorMainOverride(yylineno);
        exit(1);
    }
    if (!symbolTable.getFuncsByName(node.getId()).empty()) {
        validateMultipleDeceleration(node.getId(), symbolTable);
	} else if (!symbolTable.getCurrentOverride()) {
        validateFuncDoesntExist(node.getId(), symbolTable);
    }
}
void ValidationVisitor::visitFuncDecl(FuncDeclNode &node) {
	if (node.isOverride()) {
		auto same_id_funcs = symbolTable.getFuncsByName(node.getId()->getId());
		if (same_id_funcs.empty()) return;
        validateMultipleDeceleration(node.getId()->getId(), symbolTable);
		validatePrototypeMatch(node, same_id_funcs.back());
	}
}



void ValidationVisitor::visitId(IdNode &node) {
	validateIdIsDefined(node, symbolTable);
}

void ValidationVisitor::visitIf(IfNode &node) {
	validateIsBool(node.getCondition()->expr_type);
}

void ValidationVisitor::visitWhile(WhileNode &node) {
	validateIsBool(node.getCondition()->expr_type);
}

void ValidationVisitor::visitBinOp(BinOpNode &node) {
	validateIsNumber(node.getLeft()->expr_type);
	validateIsNumber(node.getRight()->expr_type);
}

void ValidationVisitor::visitProgram(ProgramNode &node) {
	validateMainExists(node.getFuncs());
}
