//
// Created by Snir Bachar on 06/06/2023.
//
#include <iostream>
#include <numeric>
#include <string>
#include <vector>
#include "visitor.hpp"


void CodeGenVisitor::visitProgram(ProgramNode &node) {
	string globals[] = {
			"declare i32 @printf(i8*, ...)",
			"declare void @exit(i32)",
			R"(@.int_specifier = constant [4 x i8] c"%d\0A\00")",
			R"(@.str_specifier = constant [4 x i8] c"%s\0A\00")",
			R"(@.zero_div_err = constant [24 x i8] c"Error division by zero\0A\00")"};
	for (auto &glob: globals) {
		buffer.emitGlobal(glob);
	}
	buffer.emitGlobal("define void @printi(i32) {");
	buffer.emitGlobal("\t%spec_ptr = getelementptr [4 x i8], [4 x i8]* @.int_specifier, i32 0, i32 0");
	buffer.emitGlobal(
			"\tcall i32 (i8*, ...) @printf(i8* getelementptr([4 x i8], [4 x i8]* @.int_specifier, i32 0, i32 0), i32 %0)");
	buffer.emitGlobal("\tret void");
	buffer.emitGlobal("}");
	buffer.emitGlobal("define void @print(i8*) {");
	buffer.emitGlobal("\t%spec_ptr = getelementptr [4 x i8], [4 x i8]* @.str_specifier, i32 0, i32 0");
	buffer.emitGlobal(
			"\tcall i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @.str_specifier, i32 0, i32 0), i8* %0)");
	buffer.emitGlobal("\tret void");
	buffer.emitGlobal("}");

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
		buffer.emit("%" + left_place + " = zext i8 %" + node.getLeft()->place + " to i32");
	}
	if (node.getRight()->expr_type == Types::BYTE_T) {
		right_place = buffer.newTemp();
		buffer.emit("%" + right_place + " = zext i8 %" + node.getRight()->place + " to i32");
	}
	if (op == "/") {
		string zero_div_var = buffer.newTemp();
		buffer.emit("%" + zero_div_var + " = icmp eq i32 %" + right_place + ", 0");
		int zero_div_index = buffer.emit("br i1 %" + zero_div_var + " , label @ , label @");
		auto exit_label = buffer.genLabel();
		buffer.bpatch(CodeBuffer::makelist({zero_div_index, FIRST}), exit_label);
		buffer.emit(
				"call i32 (i8*, ...) @printf(i8* getelementptr([24 x i8], [24 x i8]* @.zero_div_err, i32 0, i32 0))");
		buffer.emit("call void @exit(i32 0)");

		buffer.emit("br label %label_" + to_string(buffer.labelCounter));
		auto div_label = buffer.genLabel();
		buffer.bpatch(CodeBuffer::makelist({zero_div_index, SECOND}), div_label);
		buffer.emit("%" + node.place + " = sdiv i32 %" + left_place + ", %" + right_place);
	} else if (op == "*") {
		buffer.emit("%" + node.place + " = mul i32 %" + left_place + ", %" + right_place);
	} else if (op == "+") {
		buffer.emit("%" + node.place + " = add i32 %" + left_place + ", %" + right_place);
	} else if (op == "-") {
		buffer.emit("%" + node.place + " = sub i32 %" + left_place + ", %" + right_place);
	}
	if (node.expr_type == Types::BYTE_T) {
		auto tmp = buffer.newTemp();
		buffer.emit("%" + tmp + " = trunc i32 %" + node.place + " to i8");
		node.place = tmp;
	}

}

void CodeGenVisitor::visitVarDecl(VarDeclNode &node) {

	if (node.getExpr() == nullptr) {
		buffer.emit("%" + symbol_table.getIdSymbols(node.getId()).back()->label + " = alloca i32");
		buffer.emit("store "+node.getType()->getLlvmType()+" 0, i32* %" + symbol_table.getIdSymbols(node.getId()).back()->label);
		return;
	}
	auto expr = node.getExpr();
	if (node.getType()->getType() == Types::BOOL_T) {
		expr->place = buffer.newTemp();
		string true_label = buffer.genLabel();
		buffer.emit("br label %label_" + to_string(buffer.labelCounter + 1) + "; true ");
		string false_label = buffer.genLabel();
		buffer.emit("br label %label_" + to_string(buffer.labelCounter) + "; false ");
		buffer.genLabel();
		buffer.emit("%" + expr->place + " = phi i1 [1,%" + true_label + "], [0,%" + false_label + "]");
		buffer.bpatch(expr->true_list, true_label);
		buffer.bpatch(expr->false_list, false_label);
	}
	buffer.emit("%" + symbol_table.getIdSymbols(node.getId()).back()->label + " = alloca " + node.getExpr()->getLlvmType());
	buffer.emit(
			"store "+ node.getExpr()->getLlvmType() +" %" + node.getExpr()->place + ", "+node.getExpr()->getLlvmType()+"* %" +
			symbol_table.getIdSymbols(node.getId()).back()->label);
}

void CodeGenVisitor::visitAssign(AssignNode &node) {

	auto expr = node.getExpr();
	if (expr->expr_type == Types::BOOL_T) {
		expr->place = buffer.newTemp();
		string trueLabel = buffer.genLabel();
		int trueLine = buffer.emit("br label @; true ");
		string falseLabel = buffer.genLabel();
		int falseLine = buffer.emit("br label @; false ");
		string nextLabel = buffer.genLabel();
		buffer.emit("%" + expr->place + " = phi i32 [1,%" + trueLabel + "], [0,%" + falseLabel + "]");
		buffer.bpatch(expr->true_list, trueLabel);
		buffer.bpatch(expr->false_list, falseLabel);
		buffer.bpatch(CodeBuffer::makelist({trueLine, FIRST}), nextLabel);
		buffer.bpatch(CodeBuffer::makelist({falseLine, FIRST}), nextLabel);
	}
	buffer.emit(
			"store "+expr->getLlvmType()+" %" + expr->place + ", "+expr->getLlvmType()+"* %" + symbol_table.getIdSymbols(node.getId()).back()->label);
}

void CodeGenVisitor::visitId(IdNode &node) {
	auto id = symbol_table.getIdSymbols(node.getId()).back();
	if (id->offset >= 0) {
		auto &ptr_name = id->label;
		node.place = buffer.newTemp();
		buffer.emit("%" + node.place + " = load "+node.getLlvmType()+", "+node.getLlvmType()+"* %" + ptr_name);
	} else {
		node.place = buffer.newTemp();
		buffer.emit("%" + node.place + " = add " + id->type.getLlvmType() + " 0, %" + id->label);
	}
	if (id->type.getType() == Types::BOOL_T) {
		node.expr_type = Types::BOOL_T;
		auto index = buffer.emit("br i1 %" + node.place + ", label @, label @");
		node.true_list.emplace_back(index, FIRST);
		node.false_list.emplace_back(index, SECOND);
	}
}

void CodeGenVisitor::visitNum(NumNode &node) {
	node.place = buffer.newTemp();
	buffer.emit("%" + node.place + " = add "+node.getLlvmType()+" 0, " + std::to_string(node.getNum()));
}

void CodeGenVisitor::visitNMarker(NMarkerNode &node) {
	auto index = buffer.emit("br label @");
	node.next_list.emplace_back(index, BranchLabelIndex::FIRST);
}

void CodeGenVisitor::visitMMarker(MMarkerNode &node) {
	buffer.emit("br label %label_" + to_string(buffer.labelCounter));
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
	buffer.bpatch(node.getBlock()->next_list, node.getMMarker1()->label);
	buffer.bpatch(node.getCondition()->true_list, node.getMMarker2()->label);
	node.next_list = CodeBuffer::merge(node.getCondition()->false_list, context.getBreaks());
	buffer.emit("br label %" + node.getMMarker1()->label);
}

void CodeGenVisitor::visitStatements(StatementsNode &node) {
	if (node.getStatements() != nullptr) {
		buffer.bpatch(node.getStatements()->next_list, node.getMMarker()->label);
	}
	node.next_list = node.getStatement()->next_list;
	auto index = buffer.emit("br label @");
	node.next_list.emplace_back(index, BranchLabelIndex::FIRST);
}

void CodeGenVisitor::visitBlock(BlockNode &node) {
	node.next_list = node.getStatements()->next_list;
}

void CodeGenVisitor::visitString(StringNode &node) {
	auto &str = node.getStr();
	node.place = "str_" + buffer.newTemp();
	buffer.emit("%" + node.place + " = getelementptr [" + to_string(str.size() + 1) + " x i8], [" +
				to_string(str.size() + 1) + " x i8]* @." + node.place + "_ptr, i32 0, i32 0");
	buffer.emitGlobal(
			"@." + node.place + "_ptr" + " = constant [" + to_string(str.size() + 1) + " x i8] c\"" + str + R"(\00")");
}

void CodeGenVisitor::visitBool(BoolNode &node) {
	auto index = buffer.emit("br label @");
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
		buffer.emit("%" + left_place + " = zext i8 %" + node.getLeft()->place + " to i32");
	}
	if (node.getRight()->expr_type == Types::BYTE_T) {
		right_place = buffer.newTemp();
		buffer.emit("%" + right_place + " = zext i8 %" + node.getRight()->place + " to i32");
	}
	buffer.emit("%" + tmp + " = icmp " + node.getLlvmOp() + " i32 %" + left_place + ", %" + right_place);
	auto index = buffer.emit("br i1 %" + tmp + ", label @, label @");
	node.true_list = CodeBuffer::makelist({index, BranchLabelIndex::FIRST});
	node.false_list = CodeBuffer::makelist({index, BranchLabelIndex::SECOND});
}


void CodeGenVisitor::visitBoolExpr(BoolExprNode &node) {
	node.false_list = node.getCondition()->false_list;
	node.true_list = node.getCondition()->true_list;


}

void CodeGenVisitor::visitFormals(FormalsNode &node) {
	auto current_function = symbol_table.getAllFuncs().back();
	std::string func_decl_str("define " + current_function.type.getLlvmType() + " @" + current_function.full_name + "(");
	auto formal_list_node = current_function.formals.getFormalDecls();
	if (formal_list_node != nullptr) {
		auto args = formal_list_node->getFormalDecls();
		if (args.size() > 0) {
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
		buffer.emit("%" + default_return + " = add " + func.type.getLlvmType() + " 0, 0");
		buffer.emit("ret " + func.type.getLlvmType() + " %" + default_return);
	} else {
		if (func.name == "main") {
			buffer.emit("call void @exit(i32 0)");
		}
		buffer.emit("ret void");
	}
	buffer.emitLabel("}");
}


void CodeGenVisitor::visitCallStatement(CallStatementNode &node) {
	auto func = symbol_table.getFuncSymbol(node.getId(), node.getArgs());
	std::string call_str = "call " + func->type.getLlvmType() + " @" + func->full_name;
	auto args = node.getArgs()->getExprs();
	if (!args.empty()) {
		auto func_args = func->formals.getFormalDecls()->getFormalDecls();
		std::vector<std::string> string_list(args.size());
		auto getLabelFn = [&func_args, &args, this](int i) {
			if (args[i]->expr_type == Types::BYTE_T && func_args[i]->getType()->getType() == Types::INT_T) {
				auto new_place = buffer.newTemp();
				buffer.emit("%" + new_place + " = zext i8 %" + args[i]->place + " to i32");
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
		auto args_str = std::accumulate(string_list.begin(), string_list.end(), std::string(), addCommaFn);
		call_str += "(" + args_str + ")";
	}
	buffer.emit(call_str);
}

void CodeGenVisitor::visitCallExpr(CallExprNode &node) {
	auto func = symbol_table.getFuncSymbol(node.getId(), node.getArgs());
	std::string call_str = "call " + func->type.getLlvmType() + " @" + func->full_name;
	auto args_node = node.getArgs();
	if (args_node && !args_node->getExprs().empty()) {
		auto args = args_node->getExprs();
		auto func_args = func->formals.getFormalDecls()->getFormalDecls();
		std::vector<std::string> string_list(args.size());
		auto getLabelFn = [&func_args, &args, this](int i) {
			if (args[i]->expr_type == Types::BYTE_T && func_args[i]->getType()->getType() == Types::INT_T) {
				auto new_place = buffer.newTemp();
				buffer.emit("%" + new_place + " = zext i8 %" + args[i]->place + " to i32");
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
		auto args_str = std::accumulate(string_list.begin(), string_list.end(), std::string(), addCommaFn);
		call_str += "(" + args_str + ")";
	} else {
		call_str += "()";
	}
	node.place = buffer.newTemp();
	buffer.emit("%" + node.place + " = " + call_str);
	if (node.expr_type == Types::BOOL_T) {
		auto index = buffer.emit("br i1 %" + node.place + " , label @ , label @");
		node.true_list = CodeBuffer::makelist({index, BranchLabelIndex::FIRST});
		node.false_list = CodeBuffer::makelist({index, BranchLabelIndex::SECOND});
	}
}


void CodeGenVisitor::visitCast(CastNode &node) {
	node.place = buffer.newTemp();
	if (node.getType()->getType() == Types::INT_T) {
		buffer.emit("%" + node.place + " = add i32 0, %" + node.getExpr()->place);
	} else if (node.getType()->getType() == Types::BYTE_T) {
		auto tmp_place = buffer.newTemp();
		buffer.emit("%" + tmp_place + " = add i32 0, %" + node.getExpr()->place);
		buffer.emit("%" + node.place + " = trunc i32 %" + tmp_place + " to i8");
	} else if (node.getType()->getType() == Types::BOOL_T) {
		auto tmp_place = buffer.newTemp();
		buffer.emit("%" + tmp_place + " = add i32 0, %" + node.getExpr()->place);
		buffer.emit("%" + node.place + " = icmp ne i32 %" + tmp_place + ", 0");
	}
}

void CodeGenVisitor::visitReturn(ReturnNode &node) {
	if (node.getExpr() != nullptr) {
		auto expr = node.getExpr();
		if (expr->expr_type == Types::BOOL_T) {
			expr->place = buffer.newTemp();
			string true_label = buffer.genLabel();
			buffer.emit("br label %label_" + to_string(buffer.labelCounter + 1) + "; true ");
			string false_label = buffer.genLabel();
			buffer.emit("br label %label_" + to_string(buffer.labelCounter) + "; false ");
			buffer.genLabel();
			buffer.emit("%" + expr->place + " = phi i1 [1,%" + true_label + "], [0,%" + false_label + "]");
			buffer.bpatch(expr->true_list, true_label);
			buffer.bpatch(expr->false_list, false_label);
		}
		buffer.emit("ret " + node.getExpr()->getLlvmType() + " %" + node.getExpr()->place);
	} else {
		buffer.emit("ret void");
	}
}


void CodeGenVisitor::visitBreak(BreakNode &node) {
	auto index = buffer.emit("br label @");
	context.addBreak(index);
}

void CodeGenVisitor::visitContinue(ContinueNode &node) {
	auto index = buffer.emit("br label @");
	node.next_list = CodeBuffer::makelist({index, BranchLabelIndex::FIRST});
}

void CodeGenVisitor::visitExprList(ExprListNode &node) {
	auto expr = node.getExprs().front();
	if (expr->place.empty()) {
		if (expr->expr_type == Types::BOOL_T) {
			expr->place = buffer.newTemp();
			string true_label = buffer.genLabel();
			buffer.emit("br label %label_" + to_string(buffer.labelCounter + 1) + "; true ");
			string false_label = buffer.genLabel();
			buffer.emit("br label %label_" + to_string(buffer.labelCounter) + "; false ");
			buffer.genLabel();
			buffer.emit("%" + expr->place + " = phi i1 [1,%" + true_label + "], [0,%" + false_label + "]");
			buffer.bpatch(expr->true_list, true_label);
			buffer.bpatch(expr->false_list, false_label);
		} else {
			buffer.printCodeBuffer();
			throw std::runtime_error("non-boolean expression without place");
		}
	}
}