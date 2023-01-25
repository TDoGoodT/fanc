//
// Created by snir on 12/19/22.
//
#include "symbol_table.hpp"
#include "symbol_table_manager.hpp"
#include "hw3_output.hpp"
#include "actions.hpp"
#include <assert.h>
#include <exception>

using namespace output;


vector<SymbolTableEntry *> SymbolTable::get_all_ids() {
    vector<SymbolTableEntry *> ids;
    for (SymbolTableEntry *entry: entries_vector) {
        if (entry->which == 0) {
            ids.push_back(entry);
        }
    }
    return ids;
}

vector<SymbolTableEntry *> SymbolTable::get_all_funcs() {
    vector<SymbolTableEntry *> funcs;
    for (SymbolTableEntry *entry: entries_vector) {
        if (entry->which == 1) {
            funcs.push_back(entry);
        }
    }
    return funcs;
}
