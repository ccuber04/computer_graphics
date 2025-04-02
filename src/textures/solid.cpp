//
// Created by Conna on 3/30/2025.
//

#include "solid.h"

Solid::Solid(Color color)
    : color{color} {}

Color Solid::value(double u, double v) const {
    return color;
}