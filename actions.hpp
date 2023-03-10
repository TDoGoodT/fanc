//
// Created by snir on 12/19/22.
//

#ifndef ANALYZER_ACTIONS_HPP
#define ANALYZER_ACTIONS_HPP

#include "symbol_table_manager.hpp"
#include "store.hpp"

namespace actions {

    void start_program(Store *store);

    void begin_scope(Store *store);

    void end_scope(Store *store);

    void end_program(Store *store);

    void insert_id(Store *store, struct Id *id);

    void end_function(Store *store, struct FuncDecl *func);

    void begin_function(Store *store, Type::TypeCase type);

    void insert_function_params(Store *store, struct FormalsList *formals);

    void begin_if(Store *store);

    void begin_else(Store *store);

    void begin_while(Store *store);

    void end_while(Store *store);

    Id *get_id(Store *store, Id *id);

    Id *get_func(Store *store, Id *id);

    void declare_func(Store *store, Id *func_id);
}


#endif //ANALYZER_ACTIONS_HPP
