//
// Created by Snir Bachar on 05/06/2023.
//

#ifndef FANC_ANALYZER_PARSER_H
#define FANC_ANALYZER_PARSER_H


#include <iostream>
#include <utility>
#include <vector>
#include <deque>
#include <string>
#include "tokens.hpp"
#include "bp.hpp"

class Node {
public:
	virtual ~Node() = default;

	virtual void accept(class ParserVisitor &visitor) = 0;
};

class IfLabelNode : public Node {
public:
	void accept(class ParserVisitor &visitor) override;
};

class ElseLabelNode : public Node {
	public:
	void accept(class ParserVisitor &visitor) override;
};

class WhileLabelNode : public Node {
public:
	void accept(class ParserVisitor &visitor) override;
};


class MMarkerNode : public Node {
public:
	std::string label;
	void accept(class ParserVisitor &visitor) override;
};

class NMarkerNode : public Node {
public:
	std::vector<pair<int, BranchLabelIndex>> next_list{};
	void accept(class ParserVisitor &visitor) override;
};

class TypeNode : public Node {
private:
	Types type;
public:
	explicit TypeNode(Types type) : type(type) {}

	Types getType() const {
		return type;
	}
	std::string getLlvmType() const {
		switch (type) {
			case Types::BYTE_T:
				return "i8";
			case Types::INT_T:
				return "i32";
			case Types::BOOL_T:
				return "i1";
			case Types::VOID_T:
				return "void";
			case Types::STRING_T:
				return "i8*";
			default:
				return "unknown";
		}
	}
	std::string prettyString() const {
		switch (type) {
			case Types::BYTE_T:
				return "BYTE";
			case Types::INT_T:
				return "INT";
			case Types::BOOL_T:
				return "BOOL";
			case Types::VOID_T:
				return "VOID";
			case Types::STRING_T:
				return "STRING";
			default:
				return "unknown";
		}
	}

	void accept(class ParserVisitor &visitor) override;
};

class FormalDeclNode : public Node {
private:
	TypeNode *type;
	std::string id;

public:
	explicit FormalDeclNode(TypeNode *type, std::string id) : type(type), id(std::move(id)) {}

	TypeNode *getType() const {
		return type;
	}


	const std::string &getId() const {
		return id;
	}

	void accept(class ParserVisitor &visitor) override;
};

class FormalListNode : public Node {
private:
	std::vector<FormalDeclNode *> formalDecls;
public:
	explicit FormalListNode(FormalDeclNode *formalDecl, FormalListNode *formalDecls = nullptr) {
		this->formalDecls.push_back(formalDecl);
		if (formalDecls != nullptr) {
			for (auto &formal: formalDecls->getFormalDecls()) {
				this->formalDecls.push_back(formal);
			}
			delete formalDecls;
		}
	}

	const std::vector<FormalDeclNode *> &getFormalDecls() const {
		return formalDecls;
	}

    void accept(class ParserVisitor &visitor) override;
};

class FormalsNode : public Node {
private:
	FormalListNode *formalDecls;

public:
	explicit FormalsNode(FormalListNode *formalDecls = nullptr) : formalDecls(formalDecls) {}

	const FormalListNode *getFormalDecls() const {
		return formalDecls;
	}

	std::vector<std::string> prettyStrings() const {
		std::vector<std::string> strings;
		if (formalDecls != nullptr) {
			for (auto &formal: formalDecls->getFormalDecls()) {
				strings.push_back(formal->getType()->prettyString());
			}
		}
		return strings;
	}

	void accept(class ParserVisitor &visitor) override;
};

class RetTypeNode : public Node {
private:
	TypeNode *type;

public:
	explicit RetTypeNode(TypeNode *type) : type(type) {}

	explicit RetTypeNode(Types type) : type(new TypeNode(type)) {}

	TypeNode *getType() const {
		return type;
	}

	void accept(class ParserVisitor &visitor) override;
};

class StatementNode : public Node {
public:
	std::vector<pair<int, BranchLabelIndex>> next_list;
    StatementNode() {
        next_list = std::vector<pair<int, BranchLabelIndex>>();
    }
	void accept(class ParserVisitor &visitor) override;
};

class StatementsNode : public StatementNode {
private:
	StatementsNode* statements;
	MMarkerNode *mMarker;
	StatementNode *statement;

public:
	// statements mMarker statement { ALLOCATE_AND_ACCEPT($$, StatementsNode($1, $2, $3)); }
	explicit StatementsNode(StatementsNode *statements, MMarkerNode *mMarker, StatementNode *statement) : mMarker(mMarker), statement(statement) , statements(statements) {}
	explicit StatementsNode(StatementNode *statement) : statement(statement) , statements(nullptr) , mMarker(nullptr) {}

	StatementsNode *getStatements() const {
		return statements;
	}

	MMarkerNode *getMMarker() const {
		return mMarker;
	}

	StatementNode *getStatement() const {
		return statement;
	}
	void accept(class ParserVisitor &visitor) override;
};

class FuncIdNode : public Node {
private:
	std::string id;
public:
	explicit FuncIdNode(std::string id) : id(std::move(id)) {}

	const std::string &getId() const {
		return id;
	}

	void accept(class ParserVisitor &visitor) override;
};

class OverrideNode : public Node {
	bool override;
public:
	explicit OverrideNode(bool override) : override(override) {}

	void accept(class ParserVisitor &visitor) override;

	bool getOverride() const {
		return override;
	}
};

class FuncDeclNode : public Node {
private:
	OverrideNode *override;
	RetTypeNode *retType;
	FuncIdNode *id;
	FormalsNode *formals;
	StatementsNode *statements;

public:
	FuncDeclNode(OverrideNode *override, RetTypeNode *retType, FuncIdNode *id, FormalsNode *formals,
				 StatementsNode *statements)
			: override(override), retType(retType), id(id), formals(formals), statements(statements) {}

	bool isOverride() const {
		return override->getOverride();
	}

	RetTypeNode *getRetType() const {
		return retType;
	}

	FuncIdNode *getId() const {
		return id;
	}

	const FormalsNode *getFormals() const {
		return formals;
	}

	StatementsNode *getStatement() const {
		return statements;
	}

	void accept(class ParserVisitor &visitor) override;
};

class FuncsNode : public Node {
private:
	std::vector<const FuncDeclNode *> funcDecls;

public:
	explicit FuncsNode() = default;

	explicit FuncsNode(const FuncDeclNode *funcDecl, const FuncsNode *funcs) {
		funcDecls.push_back(funcDecl);
		for (auto &func: funcs->getFuncDecls()) {
			funcDecls.push_back(func);
		}
		delete funcs;
	}

	const std::vector<const FuncDeclNode *> &getFuncDecls() const {
		return funcDecls;
	}

	void accept(class ParserVisitor &visitor) override;
};


class ExprNode : public Node {
public:
	std::vector<std::pair<int, BranchLabelIndex>> true_list{};
	std::vector<std::pair<int, BranchLabelIndex>> false_list{};
	std::string place;
	Types expr_type = NOT_SET;

	std::string getLlvmType() const {
		switch (expr_type) {
			case Types::INT_T:
				return "i32";
			case Types::BOOL_T:
				return "i1";
			case Types::STRING_T:
				return "i8*";
			case Types::VOID_T:
				return "void";
			case Types::BYTE_T:
				return "i8";
			default:
				return "i32";
		}
	}

	void accept(class ParserVisitor &visitor) override;
};


class IdNode : public ExprNode {
private:
	std::string id;
public:
	explicit IdNode(std::string id) : id(std::move(id)) {}

	const std::string &getId() const {
		return id;
	}

	void accept(class ParserVisitor &visitor) override;
};

class ExprListNode : public Node {
private:
	std::deque<ExprNode *> exprs;
public:
	explicit ExprListNode(ExprNode *expr, ExprListNode *exprs = nullptr) {
		this->exprs.push_back(expr);
		if (exprs != nullptr) {
			for (auto &expr_itr: exprs->getExprs()) {
				this->exprs.push_back(expr_itr);
			}
			delete exprs;
		}
	}

	const std::deque<ExprNode *> &getExprs() const {
		return exprs;
	}

	void accept(class ParserVisitor &visitor) override;
};


class CallNode : public Node {
private:
	std::string id;
	ExprListNode *args;

public:
	explicit CallNode(std::string id, ExprListNode *args = nullptr) : id(std::move(id)), args(args) {}

	const std::string &getId() const {
		return id;
	}

	ExprListNode *getArgs() const {
		return args;
	}

	void accept(class ParserVisitor &visitor) override;
};

class CallStatementNode : public StatementNode, public CallNode {
public:
	explicit CallStatementNode(CallNode *call) : CallNode(call->getId(), call->getArgs()) {}

	void accept(class ParserVisitor &visitor) override;
};

class NumNode : public ExprNode {
private:
	std::string num;
	bool is_byte;
public:
	explicit NumNode(char *num, bool is_byte = false) : num(num), is_byte(is_byte) {
	}

	int getNum() const {
		return std::stoi(num);
	}

	bool isByte() const {
		return is_byte;
	}

	void accept(class ParserVisitor &visitor) override;
};

class StringNode : public ExprNode {
private:
	std::string str;
public:
	explicit StringNode(char *str) : StringNode(std::string(str)) {}

	explicit StringNode(const std::string& str) : str(str.substr(1, str.size() - 2)) {}

	const std::string &getStr() const {
		return str;
	}

	void accept(class ParserVisitor &visitor) override;

};
class BoolExprNode : public ExprNode {
private:
	ExprNode *condition;
public:

	explicit BoolExprNode(ExprNode *condition) : condition(condition) {}

	ExprNode *getCondition() const {
		return condition;
	}

	void accept(class ParserVisitor &visitor) override;
};

class BoolNode : public ExprNode {
private:
	bool val;
public:
	explicit BoolNode(bool val) : val(val) {}

	bool getVal() const {
		return val;
	}

	void accept(class ParserVisitor &visitor) override;
};

class NotNode : public ExprNode {
private:
	ExprNode *expr;
public:
	explicit NotNode(ExprNode *expr) : expr(expr) {}

	ExprNode *getExpr() const {
		return expr;
	}

	void accept(class ParserVisitor &visitor) override;
};

class CallExprNode : public ExprNode, public CallNode {
public:
	explicit CallExprNode(CallNode *call) : CallNode(call->getId(), call->getArgs()) {
		delete call;
	}

	void accept(class ParserVisitor &visitor) override;
};

class AndNode : public ExprNode {
private:
	ExprNode *left;
	MMarkerNode *mMarker;
	ExprNode *right;
public:
	explicit AndNode(ExprNode *left, MMarkerNode *mMarker, ExprNode *right) : left(left), mMarker(mMarker),
																			 right(right) {}

	ExprNode *getLeft() const {
		return left;
	}

	MMarkerNode *getMMarker() const {
		return mMarker;
	}

	ExprNode *getRight() const {
		return right;
	}

	void accept(class ParserVisitor &visitor) override;
};

class OrNode : public ExprNode {
private:
	ExprNode *left;
	MMarkerNode *mMarker;
	ExprNode *right;
public:
	explicit OrNode(ExprNode *left, MMarkerNode *mMarker, ExprNode *right) : left(left), mMarker(mMarker),
																		   right(right) {}

	ExprNode *getLeft() const {
		return left;
	}

	MMarkerNode *getMMarker() const {
		return mMarker;
	}

	ExprNode *getRight() const {
		return right;
	}

	void accept(class ParserVisitor &visitor) override;
};

class RelOpNode : public ExprNode {
private:
	ExprNode *left;
	ExprNode *right;
	std::string op;
public:
	explicit RelOpNode(ExprNode *left, std::string op, ExprNode *right) : left(left), right(right), op(std::move(op)) {}

	ExprNode *getLeft() const {
		return left;
	}

	ExprNode *getRight() const {
		return right;
	}

	const std::string &getOp() const {
		return op;
	}

	std::string getLlvmOp() const {
		if (op == "==") {
			return "eq";
		} else if (op == "!=") {
			return "ne";
		} else if (op == "<") {
			return "slt";
		} else if (op == ">") {
			return "sgt";
		} else if (op == "<=") {
			return "sle";
		} else if (op == ">=") {
			return "sge";
		} else {
			throw std::runtime_error("Unknown operator");
		}
	}

	void accept(class ParserVisitor &visitor) override;
};

class CastNode : public ExprNode {
private:
	ExprNode *expr;
	TypeNode *type;
public:
	explicit CastNode(TypeNode *type, ExprNode *expr) : expr(expr), type(type) {}

	ExprNode *getExpr() const {
		return expr;
	}

	TypeNode *getType() const {
		return type;
	}

	void accept(class ParserVisitor &visitor) override;
};

class ReturnNode : public StatementNode {
private:
	ExprNode *expr;

public:
	explicit ReturnNode(ExprNode *expr = nullptr) : expr(expr) {}

	ExprNode *getExpr() const {
		return expr;
	}

	void accept(class ParserVisitor &visitor) override;
};

class IfNode : public StatementNode {
private:
	BoolExprNode *condition;
	StatementNode *ifBlock;
	StatementNode *elseBlock{};
	MMarkerNode *mMarkerNode;
	NMarkerNode *nMarkerNode{};
	MMarkerNode *mMarkerNode2{};

public:

	// if LPAREN boolExpr RPAREN mMarker statement { ALLOCATE_AND_ACCEPT($$, IfNode($3, $5, $6)); }
	IfNode(BoolExprNode *condition, MMarkerNode *mMarkerNode, StatementNode *ifBlock) : condition(condition), ifBlock(ifBlock), mMarkerNode(mMarkerNode) {}
	// if LPAREN boolExpr RPAREN mMarker statement nMarker else mMarker statement { ALLOCATE_AND_ACCEPT($$, IfNode($3, $5, $6, $7, $9, $10)); }
	IfNode(BoolExprNode *condition, MMarkerNode *mMarkerNode, StatementNode *ifBlock, NMarkerNode *nMarkerNode, MMarkerNode *mMarkerNode2, StatementNode *elseBlock) : condition(condition), ifBlock(ifBlock), mMarkerNode(mMarkerNode), elseBlock(elseBlock), nMarkerNode(nMarkerNode), mMarkerNode2(mMarkerNode2) {}

	BoolExprNode *getCondition() const {
		return condition;
	}

	StatementNode *getIfBlock() const {
		return ifBlock;
	}

	StatementNode *getElseBlock() const {
		return elseBlock;
	}

	MMarkerNode *getMMarker() const {
		return mMarkerNode;
	}

	NMarkerNode *getNMarker() const {
		return nMarkerNode;
	}

	MMarkerNode *getMMarker2() const {
		return mMarkerNode2;
	}

	void accept(class ParserVisitor &visitor) override;
};


class WhileNode : public StatementNode {
private:
	BoolExprNode *condition;
	StatementNode *block;
	MMarkerNode *mMarker1;
	MMarkerNode *mMarker2;
public:
	// while mMarker LPAREN boolExpr RPAREN mMarker statement { ALLOCATE_AND_ACCEPT($$, WhileNode($2, $4, $6, $7)); }
	WhileNode(MMarkerNode *mMarker1, BoolExprNode *condition, MMarkerNode *mMarker2, StatementNode *block) : condition(condition), block(block), mMarker1(mMarker1), mMarker2(mMarker2) {}

	BoolExprNode *getCondition() const {
		return condition;
	}

	StatementNode *getBlock() const {
		return block;
	}

	MMarkerNode *getMMarker1() const {
		return mMarker1;
	}

	MMarkerNode *getMMarker2() const {
		return mMarker2;
	}

	void accept(class ParserVisitor &visitor) override;
};

class BreakNode : public StatementNode {
public:
	void accept(class ParserVisitor &visitor) override;
};

class ContinueNode : public StatementNode {
public:
	void accept(class ParserVisitor &visitor) override;
};

class BinOpNode : public ExprNode {
private:
	ExprNode *left;
	std::string op;
	ExprNode *right;

public:
	BinOpNode(ExprNode *left, std::string op, ExprNode *right) : left(left), op(std::move(op)), right(right) {}

	ExprNode *getLeft() const {
		return left;
	}

	const std::string &getOp() const {
		return op;
	}

	ExprNode *getRight() const {
		return right;
	}

	void accept(class ParserVisitor &visitor) override;
};


class ProgramNode : public Node {
private:
	std::vector<const FuncDeclNode *> funcs;

public:
	explicit ProgramNode(FuncsNode *funcs) {
		this->funcs = funcs->getFuncDecls();
	}

	const std::vector<const FuncDeclNode *> &getFuncs() const {
		return funcs;
	}

	void accept(class ParserVisitor &visitor) override;
};

class VarDeclNode : public StatementNode {
private:
	std::string id;
	TypeNode *type;
	ExprNode *expr;
public:
	VarDeclNode(TypeNode *type, std::string id, ExprNode *expr = nullptr) : id(std::move(id)), type(type), expr(expr) {}

	const std::string &getId() const {
		return id;
	}

	TypeNode *getType() const {
		return type;
	}

	ExprNode *getExpr() const {
		return expr;
	}

	void accept(class ParserVisitor &visitor) override;
};

class BlockNode : public StatementNode {
private:
	StatementsNode *statements;
public:
	explicit BlockNode(StatementsNode *statements) : StatementNode(), statements(statements) {}

	StatementsNode *getStatements() const {
		return statements;
	}

	void accept(class ParserVisitor &visitor) override;
};

class AssignNode : public StatementNode {
private:
	std::string id;
	ExprNode *expr;
public:
	AssignNode(std::string id, ExprNode *expr) : id(std::move(id)), expr(expr) {}

	const std::string &getId() const {
		return id;
	}

	ExprNode *getExpr() const {
		return expr;
	}

	void accept(class ParserVisitor &visitor) override;
};




#endif //FANC_ANALYZER_PARSER_H
