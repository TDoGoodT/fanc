//
// Created by snir on 12/19/22.
//

#ifndef ANALYZER_SYMBOL_TABLE
#define ANALYZER_SYMBOL_TABLE
#include "types.hpp"
#include "hw3_output.hpp"
#include <utility>
#include <vector>
#include <string>
using namespace std;
using namespace output;

struct EntrySymbolTable{
private:
    int offset;
public:
    int which;
    union _node {
        struct _T_Id *id_node;
        struct _T_FuncDecl *func_node;
        explicit _node(struct _T_Id *id_node) : id_node(id_node) {}
        explicit _node(struct _T_FuncDecl *func_node) : func_node(func_node) {}
    } node;
    
    EntrySymbolTable(struct _T_Id *id_node, int offset) : which(0), offset(offset), node(id_node) {}
    EntrySymbolTable(struct _T_FuncDecl *func_node, int offset) : which(1), offset(offset), node(func_node) {}
    string get_id() const {
        if(which == 0) return node.id_node->id;
        else return node.func_node->id->id;
    }
    string get_name() const {
        if(which==0) return node.id_node->id;
        else return node.func_node->get_name();
    }
    string get_type() const {
        if(which==0) return node.id_node->type->to_string();
        else return node.func_node->id->type->to_string();
    }
    int get_offset() const {
        return offset;
    }
};
struct SymbolTable{
    vector<EntrySymbolTable*> entries_vector;
    explicit SymbolTable() {
        entries_vector = vector<EntrySymbolTable*>();
    }
    vector<EntrySymbolTable*> get_all_ids();
    vector<EntrySymbolTable*> get_all_funcs();

};


#endif //ANALYZER_SYMBOL_TABLE
