#include "camera.h"
#include "color.h"
#include "diffuse.h"
#include "glass.h"
#include "hit.h"
#include "lambertian.h"
#include "metallic.h"
#include "parser.h"
#include "pixels.h"
#include "random.h"
#include "ray.h"
#include "specular.h"
#include "sphere.h"
#include "world.h"

#include <cmath>
#include <iomanip>

void print_progress(long long ray_num, long long total_rays);
Color trace(const World& world, const Ray& ray);
Color trace_path(const World& world, const Ray& ray, int depth);

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " {filename}";
        return 0;
    }

    Parser parser{argv[1]};
    World world = parser.get_world();
    Pixels pixels = parser.get_pixels();
    Camera camera = parser.get_camera();

    int ray_depth = parser.ray_depth;
    int samples = parser.ray_samples;

    // track progress
    const long long total_rays = static_cast<long long>(samples) * pixels.rows * pixels.columns;
    long long ray_num = 0;

    for (auto row = 0; row < pixels.rows; ++row) {
        for (auto col = 0; col < pixels.columns; ++col) {
            for (int N = 0; N < samples; ++N) {
                double y = (row + random_double()) / (pixels.rows - 1);
                double x = (col + random_double()) / (pixels.columns - 1);
                // cast samples number of rays
                Ray ray = camera.compute_ray(x, y);
                pixels(row, col) += trace_path(world, ray, ray_depth);

                ++ray_num;
                if (ray_num % (total_rays / 100) == 0) {
                    print_progress(ray_num, total_rays);
                }
            }
            pixels(row, col) /= samples;
        }
    }
    pixels.save_png(parser.filename);
    std::cout << "Wrote " << parser.filename << '\n';
}

void print_progress(long long ray_num, long long total_rays) {
    auto width = std::to_string(total_rays).length() + 4;
    int percentage = std::round(static_cast<double>(ray_num) / total_rays*100);
    std::cout << "\rProgram: " << std::setw(3) << percentage << "% ";
    std::cout << std::setw(width) << ray_num << "/" << total_rays << " rays";
    std::cout << std::flush;
}

Color trace(const World& world, const Ray& ray) {
    std::optional<Hit> hit = world.find_nearest(ray);
    if (hit.has_value()) {
        return hit->normal;
    } else {
        return Black;
    }
}

Color trace_path(const World& world, const Ray& ray, int depth) {
    if (depth == 0) {
        return Black;
    }

    std::optional<Hit> hit = world.find_nearest(ray);
    if (!hit) {
        return Black;  // artifacts, perhaps?
    }

    const Material* material = hit->sphere->material;
    if (material->emitting) {
        return material->color;
    }

    // no bounce
    Ray scattered = material->scatter(ray, hit.value());
    return material->color * trace_path(world, scattered, depth - 1);
}
