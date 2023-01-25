//
// Created by snir on 1/25/23.
//

#ifndef ANALYZER_MARKERS_HPP
#define ANALYZER_MARKERS_HPP

#include "visitor.hpp"

struct Marker {
    virtual void accept() const = 0;
};

struct InitMarker : public Marker {
    void accept() const override;
    InitMarker() {accept();}
};

#endif //ANALYZER_MARKERS_HPP
