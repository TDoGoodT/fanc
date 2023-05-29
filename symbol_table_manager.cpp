//
// Created by snir on 12/19/22.
//
#include "symbol_table.hpp"
#include "symbol_table_manager.hpp"
#include "hw3_output.hpp"
#include <cassert>

using namespace output;


SymbolTableManager::SymbolTableManager() : symbol_table_stack(), offsets_stack() {
    offsets_stack.push_back(0);
    auto *global_scope = new SymbolTable();
    symbol_table_stack.push_back(global_scope);
}

void SymbolTableManager::insert_function_params(struct FormalsList *formalsList) {
    int offset = offsets_stack.back();
    assert(offset == 0);
    for (auto param: formalsList->formalList) {
        auto *entry = new SymbolTableEntry(param->id, --offset);
        vector<SymbolTableEntry *> *const entries_vector = &symbol_table_stack.back()->entries_vector;
        entries_vector->push_back(entry);
    }
}


void SymbolTableManager::end_scope() {
    assert(offsets_stack.size() == symbol_table_stack.size());
    assert(!offsets_stack.empty());
    offsets_stack.pop_back();
    symbol_table_stack.pop_back();
}

void SymbolTableManager::begin_scope() {
    auto *new_scope = new SymbolTable();
    symbol_table_stack.push_back(new_scope);
    offsets_stack.push_back(offsets_stack.back());
}

SymbolTable *SymbolTableManager::get_current_symbol_table() {
    return symbol_table_stack.back();
}

SymbolTableEntry * SymbolTableManager::get_entry_by_name(const string &name) const {
    size_t len = symbol_table_stack.size();
    for (int i = (int) len - 1; i >= 0; i--) {
        SymbolTable *symbol_table = symbol_table_stack[i];
        for (SymbolTableEntry *entry: symbol_table->entries_vector) {
            if (entry->get_id() == name) {
                return entry;
            }
        }
    }
    return nullptr;
}

void SymbolTableManager::insert_id(Id *id) {
    int offset = offsets_stack.back();
    auto *entry = new SymbolTableEntry(id, offset);
    symbol_table_stack.back()->entries_vector.push_back(entry);
    offsets_stack.back() += 1;
}

void SymbolTableManager::insert_function(struct FuncDecl *func) {
    auto entry = new SymbolTableEntry(func, 0);
    symbol_table_stack.back()->entries_vector.push_back(entry);
}
static struct FuncDecl *createFuncDecl(string name, Type::TypeCase type) {
    auto func_id = new Id(std::move(name), new Type(Type::VOID_));
    auto inputs_decl = vector<FormalDecl *>();
    auto input_id = new Id("input", new Type(type));
    auto input_decl = new FormalDecl(input_id);
    auto func_formals_list = new FormalsList(input_decl);
    auto func_formals = new Formals(func_formals_list);
    return new FuncDecl(func_id, func_formals);
}

void SymbolTableManager::add_print_functions() {
    insert_function(createFuncDecl("print", Type::STRING_));
    insert_function(createFuncDecl("printi", Type::INT_));
}



