//
// Created by snir on 1/23/23.
//

#include <memory>
#include <string>
#include <utility>
#include "code_gen.hpp"
#include "symbol_table_manager.hpp"

using namespace code_gen;
shared_ptr<SymbolTableEntry> CodeGen::new_temp() {
    int var_id = temp_var_counter++;
    symbol_table_manager->insert_id(new_temp_id(var_id));
    return nullptr;
}

shared_ptr<Label> CodeGen::new_label() {
    return make_shared<Label>(label_counter++);;
}

void CodeGen::emit(string) {

}

CodeGen::CodeGen(shared_ptr<SymbolTableManager> manager) {
    this->symbol_table_manager = std::move(manager);
}

_T_Id *CodeGen::new_temp_id(int id) {
    return new _T_Id(to_string(id), new _T_Type(_T_Type::Type::_VOID_));
}

Label::Label(int id) {
    this->id = id;
}
