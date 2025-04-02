#include "normal.h"

Normal::Normal() {}

Color Normal::value(double u, double v) const {
    return {u, v, 1 - u*v};
}