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



vector<EntrySymbolTable*> SymbolTable::get_all_ids() {
        vector<EntrySymbolTable*> ids;
        for (EntrySymbolTable* entry : entries_vector) {
            if (entry->which == 0) {
                ids.push_back(entry);
            }
        }
        return ids;
    }
vector<EntrySymbolTable*> SymbolTable::get_all_funcs() {
    vector<EntrySymbolTable*> funcs;
    for (EntrySymbolTable* entry : entries_vector) {
        if (entry->which == 1) {
            funcs.push_back(entry);
        }
    }
    return funcs;
}
