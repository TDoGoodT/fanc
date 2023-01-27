//
// Created by snir on 1/25/23.
//

#ifndef ANALYZER_MARKERS_HPP
#define ANALYZER_MARKERS_HPP

#include "visitor.hpp"

struct Marker {
    virtual void accept() = 0;
};

struct InitMarker : public Marker {
    void accept() override;
    InitMarker() = default;
};

struct EndMarker : public Marker {
    void accept() override;
    EndMarker() = default;
};

struct OrMarker : public Marker {
    string label;
    void accept() override;
    OrMarker() = default;
};

struct AndMarker : public Marker {
    string label;
    void accept() override ;
    AndMarker() = default;
};

#endif //ANALYZER_MARKERS_HPP
