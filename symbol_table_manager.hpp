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

    vector<SymbolTable *> symbol_table_stack;
    vector<int> offsets_stack;

    void insert_id(struct Id *id);

    void insert_function_params(struct FormalsList *formalsList);

    void end_scope();

    void begin_scope();

    SymbolTable *get_current_symbol_table();

    SymbolTableEntry * get_entry_by_name(const string &name) const;

    void insert_function(struct FuncDecl *func);

    void add_print_functions();
};

#endif //ANALYZER_SYMBOL_TABLE_MANAGER_HPP

