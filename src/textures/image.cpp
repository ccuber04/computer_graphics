#include "image.h"

#include <cmath>


Image::Image(const std::string& filename)
    : pixels(filename){}

Color Image::value(double u, double v) const {
    int x = std::min(static_cast<int>(u * pixels.columns), pixels.columns - 1);
    int y = std::min(static_cast<int>(v * pixels.rows), pixels.rows - 1);

    int index = y * pixels.columns + x;
    return pixels.values[index];
}
