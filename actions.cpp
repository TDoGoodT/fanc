//
// Created by snir on 12/19/22.
//


#include "actions.hpp"
#include "types.hpp"
#include "validations.hpp"

void actions::start_program(Store *store) {
    store->symbol_table_manager.begin_scope();
    store->symbol_table_manager.add_print_functions();
}

void actions::end_program(Store *store) {
    validations::validate_main(store);
    actions::end_scope(store);
}

void actions::begin_scope(Store *store) {
    store->symbol_table_manager.begin_scope();
}

void actions::end_scope(Store *store) {
    store->symbol_table_manager.end_scope();
}

void actions::insert_id(Store *store, struct Id *id) {
    store->symbol_table_manager.insert_id(id);
}

void actions::begin_function(Store *store, Type::TypeCase type) {
    store->symbol_table_manager.begin_scope();
    store->begin_function(type);
}

void actions::insert_function_params(Store *store, struct FormalsList *formals) {
    store->crr_func_params = formals;
    store->insert_function_params(formals);
}

void actions::declare_func(Store *store, struct Id *func_id) {
    store->declare_func(func_id);
}

void actions::end_function(Store *store, struct FuncDecl *func) {
    end_scope(store);
    store->end_function(func);
}

void actions::begin_while(Store *store) {
    store->begin_while();
    begin_scope(store);
}

void actions::end_while(Store *store) {
    store->end_while();
    end_scope(store);
}

void actions::begin_if(Store *store) {
    begin_scope(store);
    end_scope(store);
}

void actions::begin_else(Store *store) {
    begin_scope(store);
    end_scope(store);
}

struct Id *actions::get_id(Store *store, struct Id *id) {
    return store->get_id(id);
}

struct Id *actions::get_func(Store *store, struct Id *id) {
    return store->get_func(id);
}