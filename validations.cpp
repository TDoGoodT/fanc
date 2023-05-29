#include "validations.hpp"
#include "symbol_table_manager.hpp"
#include "hw3_output.hpp"
#include "actions.hpp"
#include <vector>
#include <algorithm>

using namespace actions;

bool validations::is_call_correct(Store *store, struct Call *call, int lineno) {
    std::vector<FormalDecl *> func_formals;
    if (!(store->in_func && store->crr_func_id->id == call->callExp->function->id)) {
        SymbolTableEntry *func_entry = store->symbol_table_manager.get_entry_by_name(call->callExp->function->id);
        if (func_entry == nullptr || func_entry->which != 1) {
            errorUndefFunc(lineno, call->callExp->function->id);
            return false;
        }
        auto func_decl = func_entry->node.func_node;
        func_formals = func_decl->formals->formal_list->formalList;
    } else {
        func_formals =
                store->crr_func_params == nullptr ? vector<FormalDecl *>() : store->crr_func_params->formalList;
    }
    auto call_args = (call->callExp->args == nullptr) ? vector<Exp *>() : call->callExp->args->exp_list;
    // check the size first
    if (func_formals.size() != call_args.size()) {
        auto types = vector<string>();
        for (auto formal: func_formals) {
            types.push_back(formal->id->type->to_string());
        }
        errorPrototypeMismatch(lineno, call->callExp->function->id, types);
        return false;
    }
    // check the types
    for (int i = 0; i < func_formals.size(); i++) {
        auto func_formal = func_formals[i];
        auto call_arg = call_args[i];
        if (!validations::is_castable(call_arg->type->typeCase, func_formal->id->type->typeCase)) {
            auto types = vector<string>();
            for (auto formal: func_formals) {
                types.push_back(formal->id->type->to_string());
            }
            errorPrototypeMismatch(lineno, call->callExp->function->id, types);
            return false;
        }
    }
    return true;
}
void validations::validate_main(Store *store) {
    SymbolTableEntry *main_func = store->symbol_table_manager.get_entry_by_name("main");
    if (main_func == nullptr || !main_func->which || main_func->node.func_node->id->type->to_string() != "VOID" ||
        !main_func->node.func_node->formals->formal_list->formalList.empty()) {
        //error
        errorMainMissing();
        exit(1);
    }
}
bool validations::is_formal_declared(vector<struct FormalDecl *> &formal, struct Id *id) {
    return any_of(formal.begin(), formal.end(), [id](struct FormalDecl *formal) {
        return formal->id->id == id->id;
    });
}

bool validations::is_id_declared(Store *store, struct Id *id) {
    auto entry = store->symbol_table_manager.get_entry_by_name(id->id);
    if (entry == nullptr) {
        return false;
    }
    return entry->which == 0;

}

bool validations::is_func_declared(Store *store, struct Id *func_id) {
    if (!(store->in_func && store->crr_func_id != nullptr && store->crr_func_id->id == func_id->id)) {
        SymbolTableEntry *func_entry = store->symbol_table_manager.get_entry_by_name(func_id->id);
        if (func_entry == nullptr || func_entry->which != 1) {
            return false;
        }
    }
    return true;
}

bool validations::is_type_compatible(struct Exp *exp1, struct Exp *exp2) {
    return exp1->type->to_string() == exp2->type->to_string() ||
           validations::is_castable(exp2->type->typeCase, exp1->type->typeCase);
}

bool validations::has_value(struct Exp *exp) {
    return exp->value != nullptr;
}


bool validations::is_bool(struct Exp *exp) {
    return exp->type->typeCase == Type::BOOL_;
}

bool validations::is_castable(Type::TypeCase fromType, Type::TypeCase toTypeCase, bool explicit_cast) {
    if (fromType == toTypeCase) {
        return true;
    }
    if (fromType == Type::BYTE_ && toTypeCase == Type::INT_) {
        return true;
    }
    if (!explicit_cast) {
        return false;
    }
    if (toTypeCase == Type::BYTE_ && fromType == Type::INT_) {
        return true;
    }
    return false;
}

bool validations::is_byte_to_large(struct Byte *byte) {
    if (byte == nullptr) {
        return false;
    }
    return *byte->value > 255;
}
