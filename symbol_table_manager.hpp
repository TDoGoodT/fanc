//
// Created by snir on 12/19/22.
//

#ifndef ANALYZER_SYMBOL_TABLE_MANAGER_HPP
#define ANALYZER_SYMBOL_TABLE_MANAGER_HPP
#include "types.hpp"
#include "hw3_output.hpp"
#include "symbol_table.hpp"
#include <vector>
#include <string>
using namespace std;
using namespace output;

struct SymbolTableManager {
    SymbolTableManager();
    vector<SymbolTable*> symbol_table_stack;
    vector<int> offsets_stack;
    void insert_id(struct _T_Id* id);
    void insert_function_params(struct _T_FormalsList* formalsList);
    void end_scope();
    void begin_scope();
    SymbolTable* get_current_symbol_table();
    EntrySymbolTable* get_entry_by_name(const string& name);

    void insert_function(struct _T_FuncDecl* func);
};

#endif //ANALYZER_SYMBOL_TABLE_MANAGER_HPP

