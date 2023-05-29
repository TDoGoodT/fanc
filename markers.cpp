//
// Created by snir on 1/25/23.
//

#include "markers.hpp"
#include "visitor.hpp"

void InitMarker::accept() {
    Visitor::visit(this);
}
void EndMarker::accept() {
    Visitor::visit(this);
}

void OrMarker::accept() {
    Visitor::visit(this);
}


void AndMarker::accept() {
    Visitor::visit(this);
}
