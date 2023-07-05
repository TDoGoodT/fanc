//
// Created by Snir Bachar on 05/07/2023.
//

#ifndef FANC_ANALYZER_LLVM_BLOCK_H
#define FANC_ANALYZER_LLVM_BLOCK_H

#include "bp.hpp"

namespace llvm_blocks {

	std::string type_to_string(Types type) {
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
	void emit_main(CodeBuffer &buffer) {
		buffer.emitLabel("define void @main() {");
		buffer.emit("call void @fanc_main()");
		buffer.emit("call void @exit(i32 0)");
		buffer.emit("ret void");
		buffer.emitLabel("}");
	}
	void emit_program(CodeBuffer &buffer) {
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
	}

	int emit_load(CodeBuffer &buffer, const string &ptr, const string &reg) {
		return buffer.emit("%" + reg + " = load i32, i32* %" + ptr);
	}


	int emit_alloca(CodeBuffer &buffer, const string &var) {
		return buffer.emit("%" + var + " = alloca i32");
	}

	static int
	emit_op(CodeBuffer &buffer, const string &op, const string &res, const string &res_type, const string &val_a,
			const string &val_b) {
		return buffer.emit("%" + res + " = " + op + " " + res_type + " " + val_a + ", " + val_b);
	}

	int
	emit_add(CodeBuffer &buffer, const string &res, const string &res_type, const string &reg_a, const string &reg_b) {
		return emit_op(buffer, "add", res, res_type, "%" + reg_a, "%" + reg_b);
	}

	int
	emit_sub(CodeBuffer &buffer, const string &res, const string &res_type, const string &reg_a, const string &reg_b) {
		return emit_op(buffer, "sub", res, res_type, "%" + reg_a, "%" + reg_b);
	}

	int
	emit_mul(CodeBuffer &buffer, const string &res, const string &res_type, const string &reg_a, const string &reg_b) {
		return emit_op(buffer, "mul", res, res_type, "%" + reg_a, "%" + reg_b);
	}

	int
	emit_udiv(CodeBuffer &buffer, const string &res, const string &res_type, const string &reg_a, const string &reg_b) {
		return emit_op(buffer, "udiv", res, res_type, "%" + reg_a, "%" + reg_b);
	}

	int
	emit_sdiv(CodeBuffer &buffer, const string &res, const string &res_type, const string &reg_a, const string &reg_b) {
		return emit_op(buffer, "sdiv", res, res_type, "%" + reg_a, "%" + reg_b);
	}

	int
	emit_icmp(CodeBuffer &buffer, const string &res, const string &cmp_op, const string &res_type, const string &val_a,
			  const string &val_b) {
		return emit_op(buffer, "icmp " + cmp_op, res, res_type, val_a, val_b);
	}

	int
	emit_trunc(CodeBuffer &buffer, const string &res, const string &type, const string &res_type, const string &reg_a) {
		return buffer.emit("%" + res + " = trunc " + type + " %" + reg_a + " to " + res_type);
	}

	int
	emit_zext(CodeBuffer &buffer, const string &res, const string &type, const string &res_type, const string &reg_a) {
		return buffer.emit("%" + res + " = zext " + type + " %" + reg_a + " to " + res_type);
	}

	int emit_br(CodeBuffer &buffer, const string &var, const string &label_a, const string &label_b) {
		return buffer.emit("br i1 %" + var + ", label " + label_a + ", label " + label_b);
	}

	int emit_br(CodeBuffer &buffer, const string &label) {
		return buffer.emit("br label " + label);
	}

	int emit_call(CodeBuffer &buffer, const string &func, const string &res, const string &res_type,
				  const string &args_list) {
		return buffer.emit("%" + res + " = call " + res_type + " @" + func + "(" + args_list + ")");
	}

	int emit_call(CodeBuffer &buffer, const string &func, const string &res_type, const string &args_list) {
		return buffer.emit("call " + res_type + " @" + func + "(" + args_list + ")");
	}

	int emit_ret(CodeBuffer &buffer, const string &res, const string &res_type) {
		return buffer.emit("ret " + res_type + " %" + res);
	}

	int emit_ret(CodeBuffer &buffer) {
		return buffer.emit("ret void");
	}


	int emit_getelementptr(CodeBuffer &buffer, const string &res, const string &res_type, const string &ptr) {
		return buffer.emit(
				"%" + res + " = getelementptr " + res_type + ", " + res_type + "* " + ptr + ", i32 0, i32 0");
	}

	void emit_global_string(CodeBuffer &buffer, const string &ptr_name, const string &str) {
		buffer.emitGlobal(
				"@." + ptr_name + " = constant [" + to_string(str.size() + 1) + " x i8] c\"" + str + R"(\00")");
	}

	int
	emit_phi(CodeBuffer &buffer, const string &res, const string &res_type, const string &val_a, const string &label_a,
			 const string &val_b, const string &label_b) {
		return buffer.emit("%" + res + " = phi " + res_type + " [" + val_a + ", %" + label_a + "], [" + val_b + ", %" +
						   label_b + "]");
	}
	int emit_store(CodeBuffer &buffer, const string &ptr, const string &reg, const Types &type) {
		if (type == Types::BYTE_T) {
			auto new_place = buffer.newTemp();
			emit_zext(buffer, new_place, "i8", "i32", reg);
			return buffer.emit("store i32 %" + new_place + ", i32* %" + ptr);
		}
		return buffer.emit("store i32 %" + reg + ", i32* %" + ptr);
	}

}
#endif //FANC_ANALYZER_LLVM_BLOCK_H
