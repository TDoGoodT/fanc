//
// Created by snir on 12/19/22.
//


#include <utility>
#include "actions.hpp"
#include "types.hpp"
#include "symbol_table.hpp"
static struct _T_FuncDecl *createFuncDecl(string name, _T_Type::Type type) {
    auto func_id = new _T_Id(std::move(name), new _T_Type(_T_Type::_VOID_));
    auto inputs_decl = vector<_T_FormalDecl* >();
    auto input_id = new _T_Id("input", new _T_Type(type));
    auto input_decl = new _T_FormalDecl(input_id);
    auto func_formals_list = new _T_FormalsList(input_decl);
    auto func_formals = new _T_Formals(func_formals_list);
    return new _T_FuncDecl(func_id, func_formals);
}

void actions::start_program(Store* store){
    //cout << "starting program" << endl;
    store->symbol_table_manager->begin_scope();
    store->symbol_table_manager->insert_function(createFuncDecl("print", _T_Type::_STRING_));
    store->symbol_table_manager->insert_function(createFuncDecl("printi", _T_Type::_INT_));
}
void actions::end_program(Store* store){
    //cout << "finishing program" << endl;
    EntrySymbolTable* main_func = store->symbol_table_manager->get_entry_by_name("main");
    if(main_func == nullptr ||  !main_func->which || main_func->node.func_node->id->type->to_string() != "VOID" || !main_func->node.func_node->formals->formal_list->formal_list.empty()){
        //error
        errorMainMissing();
        exit(1);
    }
    actions::end_scope(store);
}
void actions::begin_scope(Store *store) {
    //cout << "new scope" << endl;
    store->symbol_table_manager->begin_scope();
}

void actions::end_scope(Store *store) {
    //cout << "end scope" << endl;
    string name;
    string str;
    int offset;
    SymbolTable* symbol_table = store->symbol_table_manager->get_current_symbol_table();
    endScope();
    for (EntrySymbolTable* entry : symbol_table->get_all_ids()){
        name = entry->get_name();
        str = entry->get_type();
        offset = entry->get_offset();
    	//print id entry
    	printID(name,offset,str);
    }
    for (EntrySymbolTable* entry : symbol_table->get_all_funcs()){
        cout << entry->get_id() << " " << entry->get_name() << " " << entry->get_offset() << endl;
    	//print function entry
    }
    store->symbol_table_manager->end_scope();
}

void actions::insert_id(Store *store, struct _T_Id* id) {
    store->symbol_table_manager->insert_id(id);
}
void actions::begin_function(Store* store, _T_Type::Type type){
    store->symbol_table_manager->begin_scope();
    store->begin_function(type);
}
void actions::insert_function_params(Store* store, struct _T_FormalsList* formals){
    store->crr_func_params = formals;
    store->insert_function_params(formals);
}
void actions::declare_func(Store* store, struct _T_Id* func_id){
    store->declare_func(func_id);
}
void actions::end_function(Store* store, struct _T_FuncDecl* func){
    end_scope(store);
    store->end_function(func);
}

void actions::begin_while(Store *store) {
    //cout << "begin while" << endl;
    store->begin_while();
    begin_scope(store);
}
void actions::end_while(Store *store) {
    //cout << "end while" << endl;
    store->end_while();
    end_scope(store);
}
void actions::begin_if(Store *store) {
    //cout << "begin if" << endl;
    begin_scope(store);
    end_scope(store);
}
void actions::begin_else(Store *store) {
    //cout << "begin else" << endl;
    begin_scope(store);
    end_scope(store);
}

struct _T_Id *actions::get_id(Store *store, struct _T_Id *id) {
    return store->get_id(id);
}
struct _T_Id *actions::get_func(Store *store, struct _T_Id *id) {
    return store->get_func(id);
}