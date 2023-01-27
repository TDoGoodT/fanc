//
// Created by snir on 12/21/22.
//

#include <cassert>
#include "store.hpp"

void Store::begin_function(Type::TypeCase type) {
    in_func = true;
    func_ret_type = type;
}

void Store::end_function(struct FuncDecl *func) {
    in_func = false;
    crr_func_id = nullptr;
    symbol_table_manager.insert_function(func);
}

void Store::begin_while() {
    in_while++;
}

Type::TypeCase Store::get_current_func_ret_type() const {
    return func_ret_type;
}

bool Store::get_in_while() const {
    return in_while > 0;
}

void Store::end_while() {
    assert(in_while > 0);
    in_while--;
}

void Store::insert_function_params(struct FormalsList *formals) {
    crr_func_params = formals;
    symbol_table_manager.insert_function_params(formals);
}


Id *Store::get_id(const string &id) const {
    auto *entry = symbol_table_manager.get_entry_by_name(id);
    if (entry == nullptr) {
        return nullptr;
    }
    if (entry->which) {
        return nullptr;
    }
    return entry->node.id_node;
}

Id *Store::get_id(struct Id *id) const {
    return this->get_id(id->id);
}

Id *Store::get_func(const string &id) const {
    if (!crr_func_id) {
        return nullptr;
    }
    if (crr_func_id->id == id) {
        return crr_func_id;
    }
    auto *entry = symbol_table_manager.get_entry_by_name(id);
    if (entry == nullptr) {
        return nullptr;
    }
    if (!entry->which) {
        return nullptr;
    }
    return entry->node.func_node->id;
}

Id *Store::get_func(Id *pId) const {
    return this->get_func(pId->id);
}

void Store::declare_func(Id *func_id) {
    assert(in_func);
    crr_func_id = func_id;
}

Store &Store::instance() {
    static Store inst;//only instance
    return inst;
}



