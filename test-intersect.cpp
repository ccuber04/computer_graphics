#include "ray.h"
#include "sphere.h"
#include "constants.h"
#include "pixels.h"
#include "color.h"
#include <optional>
#include <chrono>

void check_hit(const std::optional<double>& time, const Ray& ray);
void time_hits(const Ray& ray, const Sphere& sphere);
void draw_sphere(Pixels& pixels, const Sphere& sphere);

int main() {

    // ray testing
    Ray ray1{{0, 0, 0}, {1, 1, 0}};
    std::cout << ray1;
    for (int time = 0; time < 3; ++time) {
        auto P = ray1.at(time);
        std::cout << P << '\n';
    }

    // sphere intersection testing
    Sphere sphere{{0, 2, 0}, 1};
    Ray ray2{{1, 0, 0}, {0, 1, 0}};
    std::cout << ray2;
    std::cout << sphere << '\n';
    std::optional<double> t_alg = sphere.intersect_alg(ray2);
    std::optional<double> t_geo = sphere.intersect_geo(ray2);

    std::cout << "Algebraic intersection\n";
    check_hit(t_alg, ray2);
    std::cout << "Geometric intersection\n";
    check_hit(t_geo, ray2);

    ray2.origin += {1.1*Constants::epsilon, 0, 0};
    std::cout << "Shifted ray by epsilon in x: " << ray2;
    std::cout << "Algebraic intersection\n";
    t_alg = sphere.intersect_alg(ray2);
    check_hit(t_alg, ray2);
    std::cout << "Geometric intersection\n";
    t_geo = sphere.intersect_geo(ray2);
    check_hit(t_geo, ray2);

    //  sphere intersection timing testing
    std::cout << "\n\nChecking timings\n";
    Ray miss{{2, 0, 0}, {0, 1, 0}};
    Ray one_hit{{1, 0, 0}, {0, 1, 0}};
    Ray two_hit{{0.5, 0, 0.5}, {0, 1, 0}};
    time_hits(miss, sphere);
    time_hits(one_hit, sphere);
    time_hits(two_hit, sphere);

    // sphere printing
    const unsigned columns = 1280;
    const unsigned rows = 720;
    Pixels pixels{columns, rows};

    draw_sphere(pixels, sphere);
    std::string sphere_print{"sphere_print.png"};
    pixels.save_png(sphere_print);
    std::cout << "Wrote: " << sphere_print << '\n';
}

void check_hit(const std::optional<double>& time, const Ray& ray) {
    if (time.has_value()) {
        std::cout << "\tHit!!\n";
        std::cout << "\ttime value = " << time.value() << '\n'; // *maybe_num
        std::cout << "\tpoint at intersection = " << ray.at(time.value()) << '\n';
    }
    else { // std::nullopt
        std::cout << "\tMiss\n";
    }
}

void time_hits(const Ray& ray, const Sphere& sphere) {
    std::cout << "Algebraic intersection\n";
    std::optional<double> t_alg;
    const auto start1 = std::chrono::high_resolution_clock::now();
    t_alg = sphere.intersect_alg(ray);
    const auto end1 = std::chrono::high_resolution_clock::now();
    check_hit(t_alg, ray);
    auto duration_alg = std::chrono::duration_cast<std::chrono::nanoseconds>(end1 - start1);
    std::cout << "\tTotal time = " << duration_alg.count() << "ns\n";

    std::cout << "Geometric intersection\n";
    std::optional<double> t_geo;
    const auto start2 = std::chrono::high_resolution_clock::now();
    t_geo = sphere.intersect_alg(ray);
    const auto end2 = std::chrono::high_resolution_clock::now();
    check_hit(t_geo, ray);
    auto duration_geo = std::chrono::duration_cast<std::chrono::nanoseconds>(end2 - start2);
    std::cout << "\tTotal time = " << duration_geo.count() << "ns\n";
}

void draw_sphere(Pixels& pixels, const Sphere& sphere) {
    Ray ray{{0, 0, 0}, {0, 1, 0}};
    double scale = 0.005;

    double middle_row = (double)pixels.rows/2;
    double middle_col = (double)pixels.columns/2;

    for (int row = 0; row < pixels.rows; ++row) {
        double row_scaled = scale * (row-middle_row);
        for (int col = 0; col < pixels.columns; ++col) {
            double col_scaled = scale * (col-middle_col);
            ray.origin = {row_scaled, 0, col_scaled};

            if (sphere.intersect_geo(ray).has_value()) {
                pixels(row, col) = Aquamarine;
            } else {
                double val = static_cast<double>(col)/(pixels.columns - 1);
                Color color = {val/2, val, 1-val};
                pixels(row, col) = color;
            }
        }
    }
}
