//
// Created by snir on 1/25/23.
//

#ifndef ANALYZER_VISITOR_HPP
#define ANALYZER_VISITOR_HPP


#include <vector>
#include "types.hpp"
#include "bp.hpp"

using namespace std;
class Visitor {
public:
    static void visit(struct Exp *element);

    static void visit(struct Number *element);

    static void visit(struct Declaration *element);

    static void visit(struct Assignment *element);

    static void visit(struct LateAssignment *element);

    static void visit(struct FunctionCall *element);

    static void visit(struct Return *element);

    static void visit(struct Relop *element);

    static void visit(struct If_pattern *element);

    static void visit(struct While *element);

    static void visit(struct Statements *element);

    static void visit(struct Statement *element);

    static void visit(struct Not *element);

    static void visit(struct If *element);

    static void visit(struct Else *element);

    static void visit(struct Id *element);

    static void visit(struct String *element);

    static void visit(struct Cast *element);

    static void visit(struct Void *element);

    static void visit(struct Or *element);

    static void visit(struct And *element);

    static void visit(struct Bool *element);

    static void visit(struct RetType *element);

    static void visit(struct Program *element);

    static void visit(struct Funcs *element);

    static void visit(struct FuncDecl *element);

    static void visit(struct Formals *element);

    static void visit(struct FormalsList *element);

    static void visit(struct FormalDecl *element);

    static void visit(struct ExpList *element);

    static void visit(struct Binop *element);

    static void visit(struct Trinari *element);

    static void visit(struct Call *element);

    static void visit(struct CallExp *element);

    static void visit(struct InitMarker *element);

    static void visit(struct EndMarker *element);

    static void visit(struct OrMarker *element);

    static void visit(struct AndMarker *element);

    static void visit(struct Int *element);

    static void visit(struct Byte *element);


    static string getLlvmRelop(struct Relop* relop);



};


#endif //ANALYZER_VISITOR_HPP
