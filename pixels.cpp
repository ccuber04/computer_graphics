#include "pixels.h"
#include "lodepng.h"
#include <fstream>
#include <cmath>

Pixels::Pixels(int columns, int rows)
    :columns{columns}, rows{rows}, values(columns*rows) {}

const Color& Pixels::operator()(int row, int col) const {
    if (row >= 0 && row < rows && col >=0 && col < columns) {
        return values[row*columns + col];
    }
    else {
        throw std::out_of_range("Pixel coordinates are out of range.");
    }
}
Color& Pixels::operator()(int row, int col) {
    if (row >= 0 && row < rows && col >=0 && col < columns) {
        return values[row*columns + col];
    }
    else {
        throw std::out_of_range("Pixel coordinates are out of range.");
    }
}

void Pixels::save_ppm(const std::string& filename) {
    // use to_color<unsigned>(color.x) for printing RGB values to file
    // write the pixel values to a file using the NetBPM ppm image format
    std::ofstream ppm_file(filename);

    // verify file exists and is open
    if (!ppm_file) {
        throw std::runtime_error("Failed to open the file: " + filename + ".\n");
    }

    // write the ppm header data
    ppm_file << "P3\n"; // file format identifier
    ppm_file << columns << " " << rows << "\n"; // define dimensions of file
    ppm_file << "255\n";    // define max color value

    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < columns; col++) {
            const Color& pixel = (*this)(row, col);

            ppm_file << to_color<unsigned>(pixel.x) << " "
                     << to_color<unsigned>(pixel.y) << " "
                     << to_color<unsigned>(pixel.z) << " ";

        }
        ppm_file << "\n";
    }
    ppm_file.close();
}

void Pixels::save_png(const std::string& filename) {
    // lodepng expects pixels to be in a vector of unsigned char, where
    // the elements are R, G, B, alpha, R, G, B, alpha, etc.  Use
    // to_color<unsigned char>(color.x) for putting colors in this
    // vector, and lodepng::encode(filename, vector, width, height)
    // (make sure you check the error code returned by this function
    // using lodepng_error_text(unsigned error)).
    std::vector<unsigned char> vector;
    for (auto& color : values) {
        auto x = to_color<unsigned char>(color.x);
        auto y = to_color<unsigned char>(color.y);
        auto z = to_color<unsigned char>(color.z);
        unsigned char a = 255;
        vector.push_back(x);
        vector.push_back(y);
        vector.push_back(z);
        vector.push_back(a);
    }

    unsigned error = lodepng::encode(filename, vector, columns, rows);
    if (error) {
        std::cout << "Encoder error " << error << ": " << lodepng_error_text(error) << "\n";
    }
    else {
        std::cout << "File saved successfully: " << filename << "\n";
    }
}

double gamma_correction(double value) {
//     double gamma = 2.2;
     double gamma = 1.0;
//     double gamma = 0.7;
    return std::pow(value, 1/gamma);
}
