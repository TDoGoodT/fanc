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

void SymbolTableManager::insert_function_params(struct _T_FormalsList *formalsList) {
    int offset = offsets_stack.back();
    assert(offset == 0);
    for (auto param: formalsList->formal_list) {
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

SymbolTableEntry *SymbolTableManager::get_entry_by_name(const string &name) {
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

void SymbolTableManager::insert_id(_T_Id *id) {
    int offset = offsets_stack.back();
    auto *entry = new SymbolTableEntry(id, offset);
    symbol_table_stack.back()->entries_vector.push_back(entry);
    offsets_stack.back() += 1;
}

void SymbolTableManager::insert_function(struct _T_FuncDecl *func) {
    auto entry = new SymbolTableEntry(func, 0);
    symbol_table_stack.back()->entries_vector.push_back(entry);
}



