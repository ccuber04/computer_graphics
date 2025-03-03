#include "camera.h"
#include "color.h"
#include "diffuse.h"
#include "glass.h"
#include "hit.h"
#include "lambertian.h"
#include "metallic.h"
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

int main() {
    // world
    Diffuse purple{Purple, false};
    Diffuse light{White + White, true};
    Lambertian gray{Gray, false};
    Lambertian sapphirel{Sapphire, false};
    Lambertian bluel{Blue, false};
    Specular grays{Gray, false};
    Specular crimsons{Crimson, false};
    Glass glass{Fire, false};
    Metallic forestm{Forest, false, 0.3};

    World world;
    // left
    world.add({-1.5, 1, 0.3}, 0.5, &forestm);
    world.add({-2, -0.5, 0.3}, 0.5, &glass);
    world.add({-0.8, -1.5, 0.3}, 0.5, &bluel);

    // center
    world.add({0, 2, 0.3}, 1, &grays);

    // right
    world.add({1.5, 1, 0.3}, 0.5, &sapphirel);
    world.add({2, -0.5, 0.3}, 0.5, &purple);
    world.add({0.8, -1.5, 0.3}, 0.5, &crimsons);

    // extras
    world.add({0, 0, -100}, 100, &gray);
    world.add({100, -100, 100}, 150, &light);

    // specify the number of pixels
    Pixels pixels{1920, 1080};

    // create the camera
    Vector3D position{0, -20, 10}, up{0, 0, 1};
    Vector3D target{0, 0, 0};
    double fov{20};
    double aspect = static_cast<double>(pixels.columns) / pixels.rows;
    Camera camera{position, target, up, fov, aspect};

    constexpr int samples = 1000;
    constexpr int ray_depth = 10;

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
    std::string filename{"sphere6.png"};
    pixels.save_png(filename);
    std::cout << "Wrote " << filename << '\n';
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

    // mo bounce
    Ray scattered = material->scatter(ray, hit.value());
    return material->color * trace_path(world, scattered, depth - 1);
}
