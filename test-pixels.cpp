#include "pixels.h"
#include "color.h"
#include "constants.h"
#include <cmath>

// write a function to set all the pixels to the same color
void set_color(Pixels& pixels, Color color) {
    for (auto& pixel : pixels.values) {
        pixel = color;
    }
}

// write a function that produces a single color gradient
void gradient(Pixels& pixels) {
    for (int row = 0; row < pixels.rows; row++) {
        for (int col = 0; col < pixels.columns; col++) {
            double val = static_cast<double>(col) / (pixels.columns - 1);
            Color color = {1-val, 1-val, 1-val};

            pixels(row, col) = color;
        }
    }
}

// write a function to color a pretty picture!
void pretty(Pixels& pixels) {
    for (int row = 0; row < pixels.rows; row++) {
        for (int col = 0; col < pixels.columns; col++) {
            double col_val = static_cast<double>(col) / (pixels.columns - 1);
            double row_val = static_cast<double>(row) / (pixels.rows - 1);
            double x = pow(sin(Constants::pi * col_val), 2);
            double y = pow(sin(Constants::pi * row_val), 2);
            double z = pow(cos((col_val + row_val) / 2), 2);
            Color color = {x, y, z};

            pixels(row, col) = color;
        }
    }
}

int main() {
    const unsigned columns = 1280;
    const unsigned rows = 720;
    Pixels pixels{columns, rows};

    // example:
    set_color(pixels, White);
    std::string filename1{"white.ppm"};
    std::string filename2{"white.png"};
    pixels.save_ppm(filename1);
    pixels.save_png(filename2);
    std::cout << "Wrote: " << filename1 << '\n';
    std::cout << "Wrote: " << filename2 << '\n';

    gradient(pixels);
    std::string grad2{"grad.png"};
    pixels.save_png(grad2);
    std::cout << "Wrote: " << grad2 << '\n';

    pretty(pixels);
    std::string pretty{"pretty.png"};
    pixels.save_png(pretty);
    std::cout << "Wrote: " << pretty << '\n';
}
