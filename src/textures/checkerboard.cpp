#include "checkerboard.h"

Checkerboard::Checkerboard(Color a, Color b)
    :a{a}, b{b} {}
Color Checkerboard::value(double u, double v) const {
    int x = u*12;
    int y = v*6;
    if ((x+y) % 2 == 0) {
        return a;
    }
    else {
        return b;
    }
}