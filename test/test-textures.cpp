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
#include <thread>

void print_progress(long long ray_num, long long total_rays);
Color trace(const World& world, const Ray& ray);
Color trace_path(const World& world, const Ray& ray, int depth);
Color trace_path_iter(const World& world, const Ray& ray, int depth);
void render(const World& world, const Camera& camera, int depth, int samples, int num_threads, Pixels& pixels, bool progress=false);

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " {filename}\n";
        return 0;
    }

    try {
        Parser parser{argv[1]};
        World world = parser.get_world();
        Pixels pixels = parser.get_pixels();
        Camera camera = parser.get_camera();

        int depth = parser.ray_depth;
        int samples = parser.ray_samples;
        int num_threads = parser.num_threads;

        // Create n-1 images for additional threads
        std::vector<Pixels> images;
        for (int i = 0; i < num_threads - 1; ++i) {
            images.push_back(pixels);
        }

        // launch additional threads
        std::vector<std::thread> threads;
        for (int i = 0; i < num_threads - 1; ++i) {
            std::thread t{render, std::ref(world), camera,
                          depth, static_cast<double>(samples)/num_threads,
                          num_threads, std::ref(images.at(i)), false};
            threads.push_back(std::move(t));
        }

        // render on main thread
        render(world, camera, depth, static_cast<double>(samples)/num_threads, num_threads, pixels, true);

        // wait for other threads to finish
        for (std::thread& t : threads) {
            t.join();
        }

        // collect all image data
        for(const Pixels& p : images) {
            for (std::size_t i = 0; i < p.values.size(); ++i) {
                pixels.values.at(i) += p.values.at(i);
            }
        }

        // normalize color values by the number of threads
        for (Color& c : pixels.values) {
            c /=num_threads;
        }

        pixels.save_png(parser.filename);
        std::cout << "\nWrote " << parser.filename << '\n';
    }
    catch(std::exception& err) {
        std::cout << err.what() << '\n';
    }
}

void print_progress(long long ray_num, long long total_rays) {
    auto width = std::to_string(total_rays).length() + 4;
    int percentage = std::round(static_cast<double>(ray_num) / total_rays * 100);
    std::cout << "\rProgram: " << std::setw(3) << percentage << "% ";
    std::cout << std::setw(width) << ray_num << "/" << total_rays << " rays";
    std::cout << std::flush;
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

    const Object* sphere = hit->object;
    auto [u, v] = sphere->uv(*hit);
    const Material* material = sphere->material;
    Color color = material->texture->value(u, v);

    if (material == nullptr) {
        throw std::runtime_error("material is nullptr");
    }
    if (material->emitting) {
        return color;
    }

    // more bounces!
    Ray scattered = material->scatter(ray, hit.value());
    return color * trace_path(world, scattered, depth-1);
}

//Color trace_path_iter(const World& world, const Ray& r, int depth) {
//    Ray ray = r;
//    Color color = White;
//
//    while (depth > 0) {
//        std::optional<Hit> hit = world.find_nearest(ray);
//        if (!hit) {
//            return Black;
//        }
//
//        const Material* material = hit->sphere->material;
//        color *= material->color;
//
//        if (material->emitting) {
//            return color;
//        }
//
//        ray = material->scatter(ray, hit.value());
//
//        --depth;
//    }
//
//    if (depth == 0) {
//        return Black;
//    }
//    else {
//        return color;
//    }
//}

void render(const World& world, const Camera& camera, int depth, int samples, int num_threads, Pixels& pixels, bool progress) {
    const long long rays_total = pixels.rows * pixels.columns * static_cast<long long>(samples);
    long long ray_num = 0;

    if (progress) {
        print_progress(ray_num*num_threads, rays_total*num_threads);
    }

    for (auto row = 0; row < pixels.rows; ++row) {
        for (auto col = 0; col < pixels.columns; ++col) {
            Color color{0, 0, 0};
            for (int N = 0; N < samples; ++N) {
                double y = (row + random_double()) / (pixels.rows - 1);
                double x = (col + random_double()) / (pixels.columns - 1);
                // cast samples number of rays
                Ray ray = camera.compute_ray(x, y);
                color += trace_path(world, ray, depth);

                ++ray_num;
                if (progress && ray_num % (rays_total / 100) == 0) {
                    print_progress(ray_num*num_threads, rays_total*num_threads);
                }
            }
            pixels(row, col) = color / samples;
        }
    }
}
