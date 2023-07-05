//
// Created by Snir Bachar on 06/06/2023.
//
#include <numeric>
#include <string>
#include <vector>
#include "visitor.hpp"
#include "llvm_blocks.hpp"
#include <algorithm>

using namespace llvm_blocks;

void CodeGenVisitor::visitProgram(ProgramNode &node) {
	emit_program(buffer);
	emit_main(buffer);
	buffer.printGlobalBuffer();
	buffer.printCodeBuffer();
}

void CodeGenVisitor::visitBinOp(BinOpNode &node) {
	auto op = node.getOp();
	node.place = buffer.newTemp();
	auto left_place = node.getLeft()->place;
	auto right_place = node.getRight()->place;
	if (node.getLeft()->expr_type == Types::BYTE_T) {
		left_place = buffer.newTemp();
		emit_zext(buffer, left_place, "i8", "i32", node.getLeft()->place);
	}
	if (node.getRight()->expr_type == Types::BYTE_T) {
		right_place = buffer.newTemp();
		emit_zext(buffer, right_place, "i8", "i32", node.getRight()->place);
	}
	if (op == "/") {
		string zero_div_var = buffer.newTemp();
		emit_icmp(buffer, zero_div_var, "eq", "i32", "%" + right_place, "0");
		int zero_div_index = emit_br(buffer, zero_div_var, "@", "@");
		auto exit_label = buffer.genLabel();
		buffer.bpatch(CodeBuffer::makelist({zero_div_index, FIRST}), exit_label);
		emit_call(buffer, "printf", "i32 (i8*, ...)",
				  "i8* getelementptr([24 x i8], [24 x i8]* @.zero_div_err, i32 0, i32 0)");
		emit_call(buffer, "exit", "void", "i32 0");
		emit_br(buffer, "%label_" + to_string(buffer.labelCounter));
		auto div_label = buffer.genLabel();
		buffer.bpatch(CodeBuffer::makelist({zero_div_index, SECOND}), div_label);
		emit_sdiv(buffer, node.place, "i32", left_place, right_place);
	} else if (op == "*") {
		emit_mul(buffer, node.place, "i32", left_place, right_place);
	} else if (op == "+") {
		emit_add(buffer, node.place, "i32", left_place, right_place);
	} else if (op == "-") {
		emit_sub(buffer, node.place, "i32", left_place, right_place);
	}
	if (node.expr_type == Types::BYTE_T) {
		auto tmp = buffer.newTemp();
		emit_trunc(buffer, tmp, "i32", "i8", node.place);
		node.place = tmp;
	}

}

void CodeGenVisitor::visitVarDecl(VarDeclNode &node) {

	if (node.getExpr() == nullptr) {
		emit_alloca(buffer, symbol_table.getIdSymbols(node.getId()).back()->label);
		auto zero_var = buffer.newTemp();
		emit_op(buffer, "add", zero_var, "i32", "0", "0");
		emit_store(buffer, symbol_table.getIdSymbols(node.getId()).back()->label, zero_var, Types::INT_T);
		return;
	}
	auto expr = node.getExpr();
	buffer.emit(";" + expr->getLlvmType() + " " + node.getType()->getLlvmType());
	if (node.getType()->getType() == Types::BOOL_T && expr->expr_type == Types::BOOL_T) {
		string true_label = buffer.genLabel();
		emit_br(buffer, "%label_" + to_string(buffer.labelCounter + 1));
		string false_label = buffer.genLabel();
		emit_br(buffer, "%label_" + to_string(buffer.labelCounter));
		buffer.genLabel();
		expr->place = buffer.newTemp();
		emit_phi(buffer, expr->place, "i1", "1", true_label, "0", false_label);
		buffer.bpatch(expr->true_list, true_label);
		buffer.bpatch(expr->false_list, false_label);
		auto new_place = buffer.newTemp();
		emit_zext(buffer, new_place, "i1", "i32", expr->place);
		expr->place = new_place;
	}
	emit_alloca(buffer, symbol_table.getIdSymbols(node.getId()).back()->label);
	emit_store(buffer, symbol_table.getIdSymbols(node.getId()).back()->label, expr->place, expr->expr_type);
}

void CodeGenVisitor::visitAssign(AssignNode &node) {

	auto expr = node.getExpr();
	if (expr->expr_type == Types::BOOL_T) {
		expr->place = buffer.newTemp();
		string trueLabel = buffer.genLabel();
		int trueLine = emit_br(buffer, "@");
		string falseLabel = buffer.genLabel();
		int falseLine = emit_br(buffer, "@");
		string nextLabel = buffer.genLabel();
		emit_phi(buffer, expr->place, "i1", "1", trueLabel, "0", falseLabel);
		buffer.bpatch(expr->true_list, trueLabel);
		buffer.bpatch(expr->false_list, falseLabel);
		buffer.bpatch(CodeBuffer::makelist({trueLine, FIRST}), nextLabel);
		buffer.bpatch(CodeBuffer::makelist({falseLine, FIRST}), nextLabel);
	}
	auto id = symbol_table.getIdSymbols(node.getId()).back();
	emit_store(buffer, id->label, expr->place, expr->expr_type);
}

void CodeGenVisitor::visitId(IdNode &node) {
	auto id = symbol_table.getIdSymbols(node.getId()).back();
	if (id->offset >= 0) {
		auto &ptr_name = id->label;
		node.place = buffer.newTemp();
		emit_load(buffer, ptr_name, node.place);
		if (id->type.getType() == Types::BOOL_T) {
			auto new_place = buffer.newTemp();
			emit_trunc(buffer, new_place, "i32", "i1", node.place);
			node.place = new_place;
		}
		if (id->type.getType() == Types::BYTE_T) {
			auto new_place = buffer.newTemp();
			emit_trunc(buffer, new_place, "i32", "i8", node.place);
			node.place = new_place;
		}
	} else {
		node.place = buffer.newTemp();
		emit_op(buffer, "add", node.place, id->type.getLlvmType(), "0", "%" + id->label);
	}
	if (id->type.getType() == Types::BOOL_T) {
		node.expr_type = Types::BOOL_T;
		auto index = emit_br(buffer, node.place, "@", "@");
		node.true_list.emplace_back(index, FIRST);
		node.false_list.emplace_back(index, SECOND);
	}
}

void CodeGenVisitor::visitNum(NumNode &node) {
	node.place = buffer.newTemp();
	emit_op(buffer, "add", node.place, node.getLlvmType(), "0", std::to_string(node.getNum()));
}

void CodeGenVisitor::visitNMarker(NMarkerNode &node) {
	auto index = emit_br(buffer, "@");
	node.next_list.emplace_back(index, BranchLabelIndex::FIRST);
}

void CodeGenVisitor::visitMMarker(MMarkerNode &node) {
	emit_br(buffer, "%label_" + to_string(buffer.labelCounter));
	node.label = buffer.genLabel();
}

void CodeGenVisitor::visitIf(IfNode &node) {
	buffer.bpatch(node.getCondition()->true_list, node.getMMarker()->label);
	if (node.getElseBlock() != nullptr) {
		buffer.bpatch(node.getCondition()->false_list, node.getMMarker2()->label);
		auto temp = CodeBuffer::merge(node.getIfBlock()->next_list, node.getNMarker()->next_list);
		node.next_list = CodeBuffer::merge(temp, node.getElseBlock()->next_list);
		return;
	}
	node.next_list = CodeBuffer::merge(node.getCondition()->false_list, node.getIfBlock()->next_list);
}

void CodeGenVisitor::visitWhile(WhileNode &node) {
	auto next_list = CodeBuffer::merge(context.getContinues(), node.getBlock()->next_list);
	buffer.bpatch(next_list, node.getMMarker1()->label);
	buffer.bpatch(node.getCondition()->true_list, node.getMMarker2()->label);
	node.next_list = CodeBuffer::merge(node.getCondition()->false_list, context.getBreaks());
	emit_br(buffer, "%" + node.getMMarker1()->label);
}

void CodeGenVisitor::visitStatements(StatementsNode &node) {
	if (node.getStatements() != nullptr && !node.getStatements()->next_list.empty()) {
		buffer.bpatch(node.getStatements()->next_list, node.getMMarker()->label);
	}
	node.next_list = node.getStatement()->next_list;
	auto index = emit_br(buffer, "@");
	node.next_list.emplace_back(index, BranchLabelIndex::FIRST);
}

void CodeGenVisitor::visitBlock(BlockNode &node) {
	node.next_list = node.getStatements()->next_list;
}

void CodeGenVisitor::visitString(StringNode &node) {
	auto &str = node.getStr();
	node.place = "str_" + buffer.newTemp();
	emit_getelementptr(buffer, node.place, "[" + to_string(str.size() + 1) + " x i8]", "@." + node.place + "_ptr");
	emit_global_string(buffer, node.place + "_ptr", str);
}

void CodeGenVisitor::visitBool(BoolNode &node) {
	auto index = emit_br(buffer, "@");
	if (node.getVal()) node.true_list = CodeBuffer::makelist({index, BranchLabelIndex::FIRST});
	else node.false_list = CodeBuffer::makelist({index, BranchLabelIndex::FIRST});
}

void CodeGenVisitor::visitNot(NotNode &node) {
	node.false_list = node.getExpr()->true_list;
	node.true_list = node.getExpr()->false_list;
}

void CodeGenVisitor::visitAnd(AndNode &node) {
	buffer.bpatch(node.getLeft()->true_list, node.getMMarker()->label);
	node.true_list = node.getRight()->true_list;
	node.false_list = CodeBuffer::merge(node.getLeft()->false_list, node.getRight()->false_list);
}

void CodeGenVisitor::visitOr(OrNode &node) {
	buffer.bpatch(node.getLeft()->false_list, node.getMMarker()->label);
	node.true_list = CodeBuffer::merge(node.getLeft()->true_list, node.getRight()->true_list);
	node.false_list = node.getRight()->false_list;
}

void CodeGenVisitor::visitRelOp(RelOpNode &node) {
	auto tmp = buffer.newTemp();
	auto left_place = node.getLeft()->place;
	auto right_place = node.getRight()->place;
	if (node.getLeft()->expr_type == Types::BYTE_T) {
		left_place = buffer.newTemp();
		emit_zext(buffer, left_place, "i8", "i32", node.getLeft()->place);
	}
	if (node.getRight()->expr_type == Types::BYTE_T) {
		right_place = buffer.newTemp();
		emit_zext(buffer, right_place, "i8", "i32", node.getRight()->place);
	}
	emit_icmp(buffer, tmp, node.getLlvmOp(), "i32", "%" + left_place, "%" + right_place);
	auto index = emit_br(buffer, tmp, "@", "@");
	node.true_list = CodeBuffer::makelist({index, BranchLabelIndex::FIRST});
	node.false_list = CodeBuffer::makelist({index, BranchLabelIndex::SECOND});
}


void CodeGenVisitor::visitBoolExpr(BoolExprNode &node) {
	node.false_list = node.getCondition()->false_list;
	node.true_list = node.getCondition()->true_list;


}

void CodeGenVisitor::visitFormals(FormalsNode &node) {
	auto current_function = symbol_table.getAllFuncs().back();
	string func_name;
	if (current_function.full_name == "main") {
		func_name = "fanc_main";
	} else {
		func_name = current_function.full_name;
	}
	std::string func_decl_str(
			"define " + current_function.type.getLlvmType() + " @" + func_name + "(");
	auto formal_list_node = current_function.formals.getFormalDecls();
	if (formal_list_node != nullptr) {
		auto args = formal_list_node->getFormalDecls();
		if (!args.empty()) {
			std::vector<std::string> string_list(args.size());
			std::transform(args.begin(), args.end(), string_list.begin(),
						   [](const FormalDeclNode *expr) { return expr->getType()->getLlvmType(); });
			func_decl_str += std::accumulate(string_list.begin(), string_list.end(), std::string(),
											 [](const std::string &a, const std::string &b) {
												 if (a.empty()) { return b; }
												 else {
													 return a + ", " + b;
												 }
											 });
		}
	}

	func_decl_str += ") {";
	buffer.emitLabel(func_decl_str);
}

void CodeGenVisitor::visitFuncDecl(FuncDeclNode &node) {
	auto func_end_label = buffer.genLabel();
	buffer.bpatch(node.getStatement()->next_list, func_end_label);
	auto func = symbol_table.getAllFuncs().back();
	auto default_return = buffer.newTemp();
	if (func.type.getType() != Types::VOID_T) {
		emit_op(buffer, "add", default_return, func.type.getLlvmType(), "0", "0");
		emit_ret(buffer, default_return, func.type.getLlvmType());
	} else {
		if (func.name == "main") {
			emit_call(buffer, "exit", "void", "i32 0");
		}
		emit_ret(buffer);
	}
	buffer.emitLabel("}");
}


void CodeGenVisitor::visitCallStatement(CallStatementNode &node) {
	auto func = symbol_table.getFuncSymbol(node.getId(), node.getArgs());
	auto args = node.getArgs();
	string args_str;
	if (args != nullptr && !args->getExprs().empty()) {
		auto exprs = args->getExprs();
		auto func_args = func->formals.getFormalDecls()->getFormalDecls();
		std::vector<std::string> string_list(exprs.size());
		auto getLabelFn = [&func_args, &exprs, this](int i) {
			auto func_llvm_type = func_args[i]->getType()->getLlvmType();
			if (exprs[i]->expr_type == Types::BYTE_T && func_args[i]->getType()->getType() == Types::INT_T) {
				auto new_place = buffer.newTemp();
				emit_zext(buffer, new_place, exprs[i]->getLlvmType(), func_llvm_type, exprs[i]->place);
				return func_llvm_type + " %" + new_place;
			}
			return func_llvm_type + " %" + exprs[i]->place;
		};
		auto addCommaFn = [](const std::string &a, const std::string &b) {
			if (a.empty()) { return b; } else { return a + ", " + b; }
		};
		auto range = vector<int>(exprs.size());
		std::iota(range.begin(), range.end(), 0);
		std::transform(range.begin(), range.end(), string_list.begin(), getLabelFn);
		args_str = std::accumulate(string_list.begin(), string_list.end(), std::string(), addCommaFn);
	}
	emit_call(buffer, func->full_name, func->type.getLlvmType(), args_str);
}

void CodeGenVisitor::visitCallExpr(CallExprNode &node) {
	auto func = symbol_table.getFuncSymbol(node.getId(), node.getArgs());
	auto args_node = node.getArgs();
	string args_str;
	if (args_node && !args_node->getExprs().empty()) {
		auto args = args_node->getExprs();
		auto func_args = func->formals.getFormalDecls()->getFormalDecls();
		std::vector<std::string> string_list(args.size());
		auto getLabelFn = [&func_args, &args, this](int i) {
			if (args[i]->expr_type == Types::BYTE_T && func_args[i]->getType()->getType() == Types::INT_T) {
				auto new_place = buffer.newTemp();
				emit_zext(buffer, new_place, args[i]->getLlvmType(), func_args[i]->getType()->getLlvmType(),
						  args[i]->place);
				return "i32 %" + new_place;
			}
			return func_args[i]->getType()->getLlvmType() + " %" + args[i]->place;
		};
		auto addCommaFn = [](const std::string &a, const std::string &b) {
			if (a.empty()) { return b; } else { return a + ", " + b; }
		};
		auto range = vector<int>(args.size());
		std::iota(range.begin(), range.end(), 0);
		std::transform(range.begin(), range.end(), string_list.begin(), getLabelFn);
		args_str = std::accumulate(string_list.begin(), string_list.end(), std::string(), addCommaFn);
	}
	node.place = buffer.newTemp();
	emit_call(buffer, func->full_name, node.place, func->type.getLlvmType(), args_str);
	if (node.expr_type == Types::BOOL_T) {
		auto index = emit_br(buffer, node.place, "@", "@");
		node.true_list = CodeBuffer::makelist({index, BranchLabelIndex::FIRST});
		node.false_list = CodeBuffer::makelist({index, BranchLabelIndex::SECOND});
	}
}


void CodeGenVisitor::visitCast(CastNode &node) {
	node.place = buffer.newTemp();
	if (node.getType()->getType() == Types::INT_T) {
		emit_zext(buffer, node.place, node.getExpr()->getLlvmType(), "i32", node.getExpr()->place);
	} else if (node.getType()->getType() == Types::BYTE_T) {
		auto tmp_place = buffer.newTemp();
		emit_trunc(buffer, node.place, "i32", "i8", node.getExpr()->place);
	} else if (node.getType()->getType() == Types::BOOL_T) {
		auto tmp_place = buffer.newTemp();
		emit_icmp(buffer, node.place, "ne", "i32", "%" + node.getExpr()->place, "0");
	}
}

void CodeGenVisitor::visitReturn(ReturnNode &node) {
	if (node.getExpr() != nullptr) {
		auto expr = node.getExpr();
		if (expr->expr_type == Types::BOOL_T) {
			expr->place = buffer.newTemp();
			string true_label = buffer.genLabel();
			emit_br(buffer, "%label_" + to_string(buffer.labelCounter + 1));
			string false_label = buffer.genLabel();
			emit_br(buffer, "%label_" + to_string(buffer.labelCounter));
			buffer.genLabel();
			emit_phi(buffer, expr->place, "i1", "1", true_label, "0", false_label);
			buffer.bpatch(expr->true_list, true_label);
			buffer.bpatch(expr->false_list, false_label);
		} else if (expr->expr_type == Types::BYTE_T && symbol_table.getCurrentRetType() == Types::INT_T) {
			TypeNode type(Types::INT_T);
			CastNode cast(&type, expr);
			visitCast(cast);
			expr->place = cast.place;
		} else if (expr->expr_type == Types::INT_T && symbol_table.getCurrentRetType() == Types::BYTE_T) {
			TypeNode type(Types::BYTE_T);
			CastNode cast(&type, expr);
			visitCast(cast);
			expr->place = cast.place;
		}
		emit_ret(buffer, expr->place, type_to_string(symbol_table.getCurrentRetType()));
	} else {
		emit_ret(buffer);
	}
}


void CodeGenVisitor::visitBreak(BreakNode &node) {
	auto index = emit_br(buffer, "@");
	context.addBreak(index);
}

void CodeGenVisitor::visitContinue(ContinueNode &node) {
	auto index = emit_br(buffer, "@");
	context.addContinue(index);
}

void CodeGenVisitor::visitExprList(ExprListNode &node) {
	auto expr = node.getExprs().front();
	if (expr->place.empty()) {
		if (expr->expr_type == Types::BOOL_T) {
			expr->place = buffer.newTemp();
			string true_label = buffer.genLabel();
			emit_br(buffer, "%label_" + to_string(buffer.labelCounter + 1));
			string false_label = buffer.genLabel();
			emit_br(buffer, "%label_" + to_string(buffer.labelCounter));
			buffer.genLabel();
			emit_phi(buffer, expr->place, "i1", "1", true_label, "0", false_label);
			buffer.bpatch(expr->true_list, true_label);
			buffer.bpatch(expr->false_list, false_label);
		}
	}
}