//
// Created by snir on 1/23/23.
//
#include <string>
#include "symbol_table.hpp"
#include "symbol_table_manager.hpp"

namespace code_gen {
    struct Label {
        explicit Label(int i);
        int id;
    };

    struct CodeGen {
        CodeGen(shared_ptr<SymbolTableManager> manager);
        shared_ptr<Label> new_label();
        void emit(string);
        shared_ptr<SymbolTableEntry> new_temp();
    private:
        int temp_var_counter = 0;
        int label_counter = 0;
        shared_ptr<SymbolTableManager> symbol_table_manager;

        static _T_Id *new_temp_id(int id);
    };
}