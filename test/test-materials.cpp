#include "camera.h"
#include "sphere.h"
#include "color.h"
#include "pixels.h"
#include "ray.h"
#include "hit.h"
#include "material.h"
#include "world.h"
#include "random.h"
#include "parser.h"
#include <iomanip>
#include <cmath>

void print_progress(long long ray_num, long long total_rays);
Color trace(const World& world, const Ray& ray);
Color trace_path(const World& world, const Ray& ray, int depth);
Color trace_path_iter(const World& world, const Ray& ray, int depth);

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " {filename}\n";
        return 0;
    }

    Parser parser{argv[1]};
    World world = parser.get_world();
    Pixels pixels = parser.get_pixels();
    Camera camera = parser.get_camera();

    int depth = parser.ray_depth;
    int samples = parser.ray_samples;

    // track progress
    const long long rays_total = pixels.rows * pixels.columns * static_cast<long long>(samples);
    long long ray_num = 0;
    
    for (auto row = 0; row < pixels.rows; ++row) {
	for (auto col = 0; col < pixels.columns; ++col) {
            for (int N = 0; N < samples; ++N) {
                double y = (row + random_double()) / (pixels.rows - 1);
                double x = (col + random_double()) / (pixels.columns - 1);
                // cast samples number of rays
                Ray ray = camera.compute_ray(x, y);
                pixels(row, col) += trace_path(world, ray, depth);

                ++ray_num;
                if (ray_num % (rays_total / 100) == 0) {
                    print_progress(ray_num, rays_total);
                }
            }
            pixels(row, col) /= samples;
	}
    }

    pixels.save_png(parser.filename);
    std::cout << "\nWrote " << parser.filename << '\n';
}




Color trace(const World& world, const Ray& ray) {
    std::optional<Hit> hit = world.find_nearest(ray);
    if (hit.has_value()) {
	return hit->normal;
    }
    else {
	return Black;
    }
}

Color trace_path(const World& world, const Ray& ray, int depth) {
    if (depth == 0) {
        return Black;
    }

    std::optional<Hit> hit = world.find_nearest(ray);
    if (!hit) {
        return Black;
    }

    const Material* material = hit->sphere->material;
    if (material == nullptr) {
        throw std::runtime_error("material is nullptr");
    }
    if (material->emitting) {
        return material->color;
    }

    // more bounces!
    Ray scattered = material->scatter(ray, hit.value());
    return material->color * trace_path(world, scattered, depth-1);
}

Color trace_path_iter(const World& world, const Ray& r, int depth) {
    Ray ray = r;
    Color color = White;
    
    while (depth > 0) {
        std::optional<Hit> hit = world.find_nearest(ray);
        if (!hit) {
            return Black;
        }

        const Material* material = hit->sphere->material;
        color *= material->color;
        
        if (material->emitting) {
            return color;
        }
        
        ray = material->scatter(ray, hit.value());
        
        --depth;
    }
    
    if (depth == 0) {
        return Black;
    }
    else {
        return color;
    }
}

void print_progress(long long ray_num, long long total_rays) {
    auto width = std::to_string(total_rays).length() + 4;
    int percentage = std::round(static_cast<double>(ray_num) / total_rays * 100);
    std::cout << "\rProgram: " << std::setw(3) << percentage << "% ";
    std::cout << std::setw(width) << ray_num << "/" << total_rays << " rays";
    std::cout << std::flush;
}
