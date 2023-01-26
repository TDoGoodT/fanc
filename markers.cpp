//
// Created by snir on 1/25/23.
//

#include "markers.hpp"

void InitMarker::accept() {
    Visitor::instance().visit(this);
}
