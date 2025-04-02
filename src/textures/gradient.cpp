#include "gradient.h"

#include <cmath>


Gradient::Gradient(Color a, Color b)
    : a{a}, b{b} {}

Color Gradient::value(double u, double v) const {
    return (1-v*v)*a + v*v*b;
}