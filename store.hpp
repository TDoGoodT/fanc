//
// Created by snir on 12/21/22.
//

#ifndef ANALYZER_STORE_HPP
#define ANALYZER_STORE_HPP

#include "symbol_table_manager.hpp"

struct Store {
    SymbolTableManager symbol_table_manager = SymbolTableManager();
    int in_while = 0;
    bool in_func;
    Id *crr_func_id{};
    FormalsList *crr_func_params{};
    Type::TypeCase func_ret_type;
    static Store &instance();
    explicit Store() : in_while(0), in_func(false), func_ret_type() {}

    void begin_function(Type::TypeCase type);

    void begin_while();

    Type::TypeCase get_current_func_ret_type() const;

    bool get_in_while() const;

    void end_function(struct FuncDecl *func);

    void end_while();

    Id *get_id(Id *id) const;

    Id *get_id(const string &id) const;

    Id *get_func(Id *pId) const;

    Id *get_func(const string &id) const;

    void declare_func(Id *func_id);

    void insert_function_params(FormalsList *formals);
};


#endif //ANALYZER_STORE_HPP
