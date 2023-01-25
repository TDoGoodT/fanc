//
// Created by snir on 12/21/22.
//

#ifndef ANALYZER_STORE_HPP
#define ANALYZER_STORE_HPP

#include "symbol_table_manager.hpp"

struct Store {
    SymbolTableManager *symbol_table_manager;
    int in_while = 0;
    bool in_func;
    _T_Id *crr_func_id{};
    _T_FormalsList *crr_func_params{};
    _T_Type::Type func_ret_type;

    explicit Store(SymbolTableManager *symbol_table_manager) : symbol_table_manager(symbol_table_manager), in_while(0),
                                                               in_func(false), func_ret_type() {}

    void begin_function(_T_Type::Type type);

    void begin_while();

    _T_Type::Type get_current_func_ret_type() const;

    bool get_in_while() const;

    void end_function(struct _T_FuncDecl *func);

    void end_while();

    _T_Id *get_id(_T_Id *id) const;

    _T_Id *get_id(const string &id) const;

    _T_Id *get_func(_T_Id *pId) const;

    _T_Id *get_func(const string &id) const;

    void declare_func(_T_Id *func_id);

    void insert_function_params(_T_FormalsList *formals);
};


#endif //ANALYZER_STORE_HPP
