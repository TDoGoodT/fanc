//
// Created by Snir Bachar on 06/06/2023.
//

#ifndef FANC_ANALYZER_SYMBOL_TABLE_H
#define FANC_ANALYZER_SYMBOL_TABLE_H

#include <utility>
#include <deque>

#include "nodes.hpp"

class WhileContext {
	int counter;
	std::vector<pair<int, BranchLabelIndex>> lastLabels;
	std::vector<pair<int, BranchLabelIndex>> breakLabels;
public:
	WhileContext() : counter(0) {}

	bool inWhile() const {
		return counter > 0;
	}

	void enterWhile() {
		counter++;
	}

	void addBreak(int index) {
		breakLabels.emplace_back(index, BranchLabelIndex::FIRST);
	}

	std::vector<pair<int, BranchLabelIndex>> getBreaks() {
		return lastLabels;
	}

	void exitWhile() {
		lastLabels = breakLabels;
		breakLabels.clear();
		counter--;
	}
};

class Symbol {
};

class IdSymbol : public Symbol {
public:
	const std::string name;
	const TypeNode type;
	const int offset;
	const std::string label;

	IdSymbol(const std::string &name, const TypeNode &type, int offset, std::string label) : name(name), type(type),
																							 offset(offset),
																							 label(label) {}
};
static inline string to_string(const std::vector<std::string> &strings) {
	std::string result;
	for (auto &str: strings) {
		result += str + "_";
	}
	return result;
}
class FuncSymbol : public Symbol {
public:
	const std::string name;
	const TypeNode type;
	const FormalsNode formals;
	const bool override;
	std::string full_name;

	FuncSymbol(const std::string &name, const TypeNode &type, const FormalsNode &formals, const bool override) :
			name(name), type(type), formals(formals), override(override), full_name(name) {
		if (override)
			full_name = name + "_" + to_string(formals.prettyStrings()) + "_override";

	}
};

typedef std::deque<IdSymbol> Scope;

class SymbolTable {
	struct {
		TypeNode type;
		std::string name;
		FormalsNode formals;
		bool override;
	} current_function{
			TypeNode(Types::VOID_T),
			"",
			FormalsNode(),
			false
	};
	int id_counter = 0;

	void pushFuncSymbol() {
		newScope();
		addFuncSymbol(current_function.name, current_function.type, current_function.formals,
					  current_function.override);
		auto formals = current_function.formals.getFormalDecls();
		if (formals) {
			addIdSymbolFromFormals(formals);
		}
		current_function = {
				TypeNode(Types::VOID_T),
				"",
				FormalsNode(),
				false
		};
	}

	void addIdSymbolFromFormals(const FormalListNode *formal_decls) {
		auto &formal_list = formal_decls->getFormalDecls();
		for (int i = 0; i < formal_list.size(); ++i) {
			auto &formal = formal_list[i];
			IdSymbol id_symbol(formal->getId(), *formal->getType(), -(i + 1), to_string(i));
			scopes_stack.back().push_back(id_symbol);
		}
	}

	void addFuncSymbol(const std::string &name, const TypeNode &type, const FormalsNode &formals, bool override) {
		FuncSymbol symbol(name, type, formals, override);
		funcs.emplace_back(symbol);
		auto &current_scope = scopes_stack.back();
		const FormalListNode *formal_decls = formals.getFormalDecls();
	}

	void addDefaultFunctions() {
		auto print_formals = new FormalListNode(new FormalDeclNode(new TypeNode(Types::STRING_T), "s"));
		addFuncSymbol("print", TypeNode(Types::VOID_T), FormalsNode(print_formals), false);
		auto printi_formals = new FormalListNode(new FormalDeclNode(new TypeNode(Types::INT_T), "n"));
		addFuncSymbol("printi", TypeNode(Types::VOID_T), FormalsNode(printi_formals), false);
	}

	int allocateSpace() {
		id_counter++;
		for (auto it = scopes_stack.rbegin(); it != scopes_stack.rend(); ++it) {
			auto &scope = *it;
			if (!scope.empty() && scope.back().offset > 0) {
				return scope.back().offset + 1;
			}
		}
		return 0;
	}

public:

	SymbolTable() {
		newScope();
		addDefaultFunctions();
	}

	void newScope() {
		scopes_stack.emplace_back();
	}

	std::vector<FuncSymbol> getAllFuncs() {
		std::vector<FuncSymbol> all_funcs = funcs;
		return all_funcs;
	}

	const Scope &getCurrentScope() {
		return scopes_stack.back();
	}

	void endScope() {
		auto &scope = scopes_stack.back();
		scopes_stack.pop_back();
	}

	void setCurrentRetType(const TypeNode &type) {
		current_function.type = type;
	}

	Types getCurrentRetType() const {
		return funcs.back().type.getType();
	}

	bool getCurrentOverride() const {
		return funcs.back().override;
	}

	void setCurrentFuncId(const std::string &name) {
		current_function.name = name;
	}

	void setCurrentFormals(FormalsNode &formals) {
		current_function.formals = formals;
		pushFuncSymbol();
	}

	void setCurrentOverride(bool override) {
		current_function.override = override;
	}

	bool isOverride(const std::string &id) {
		for (auto &func: funcs) {
			if (func.name == id && func.override) {
				return true;
			}
		}
		return false;
	}

	void addIdSymbol(const std::string &name, const TypeNode &type) {
		auto &scope = scopes_stack.back();
		int offset = allocateSpace();
		IdSymbol symbol(name, type, offset, "id_" + name + "_" + std::to_string(id_counter));
		scope.emplace_back(symbol);
	}

	std::vector<FuncSymbol *> getFuncsByName(std::string id) {
		std::vector<FuncSymbol *> vector;
		for (auto &func: funcs) {
			if (func.name == id) {
				vector.push_back(&func);
			}
		}
		return vector;
	}

	FuncSymbol *getFuncSymbol(std::string id, ExprListNode *args) {
		for (auto &func: funcs) {
			if (func.name == id) {
				if (func.formals.getFormalDecls() == nullptr && args == nullptr) {
					return &func;
				}
				if (func.formals.getFormalDecls() == nullptr || args == nullptr) {
					continue;
				}
				auto &formal_list = func.formals.getFormalDecls()->getFormalDecls();
				auto &args_list = args->getExprs();
				if (formal_list.size() != args_list.size()) {
					continue;
				}
				bool match = true;
				for (int i = 0; i < formal_list.size(); ++i) {
					auto &formal = formal_list[i];
					auto &arg = args_list[i];
					if (!types_match(arg->expr_type, formal->getType()->getType())) {
						match = false;
						break;
					}
				}
				if (match) {
					return &func;
				}
			}
		}
		return nullptr;
	}

	IdSymbol *firstIdSymbol(std::string id) {
		for (auto it = scopes_stack.rbegin(); it != scopes_stack.rend(); ++it) {
			for (auto &symbol: *it) {
				if (symbol.name == id) {
					return &symbol;
				}
			}
		}
		return nullptr;
	}

	std::vector<IdSymbol *> getIdSymbols(const std::string &id) {
		std::vector<IdSymbol *> vector;
		for (auto &scope: scopes_stack) {
			for (auto &symbol: scope) {
				if (symbol.name == id) {
					vector.push_back(&symbol);
				}
			}
		}
		return vector;
	}

private:
	std::vector<Scope> scopes_stack;
	std::vector<FuncSymbol> funcs;
};

#endif //FANC_ANALYZER_SYMBOL_TABLE_H
