#include "pixels.h"
#include "sphere.h"
#include "camera.h"

#include <vector>

void double_spheres(const Camera& camera, const Sphere& sphere, const Sphere& ground, Pixels& pixels);

int main() {
    // ground sphere and surface sphere
    Sphere ground{{0, 0, -100}, 100};
    Sphere sphere{{0, 0, 1}, 1};

    // pixels
    int horizontal = 1280;
    int vertical = 720;
    Pixels pixels{horizontal, vertical};

    // create the cameras
    // define three camera positions
    Vector3D position1{0, -10, 10}, position2{0, 10, 10}, position3{0, 10, 0};

    Vector3D up{0, 0, 1};
    Vector3D target{sphere.center};
    double fov{90};
    double aspect = static_cast<double>(horizontal)/vertical;
    Camera camera1{position1, target, up, fov, aspect}; // negative y value with 10 z value
    Camera camera2{position2, target, up, fov, aspect}; // positive y value with 10 z value
    Camera camera3{position3, target, up, fov, aspect}; // negative y value 1 z value

    std::vector<Camera> cameras{camera1, camera2, camera3};

    // sphere position changes
    for (size_t i = 0; i < cameras.size(); ++i) {
        // painting the scene
        double_spheres(cameras[i], sphere, ground, pixels);

        // save to png
        // spheres0.png has camera position of position1
        // spheres1.png has camera position of position2
        // spheres2.png has camera position of position3
        std::string spheres_png{"spheres" + std::to_string(i) + ".png"};
        pixels.save_png(spheres_png);
        std::cout << "Wrote to: " << spheres_png << "\n";
    }

    // two different fov values
    double fov1{45}, fov2{180};
    Camera camera4{position2, target, up, fov1, aspect}; // 45 degree fov
    Camera camera5{position2, target, up, fov2, aspect}; // 180 degree fov
    std::vector<Camera> cameras_fov{camera4, camera5};

    // different fov values
    for (size_t i = 0; i < cameras_fov.size(); ++i) {
        // painting the scene
        double_spheres(cameras_fov[i], sphere, ground, pixels);

        // save to png
        // spheres3.png has fov of 45 degrees
        // spheres4.png has fov of 180 degrees
        std::string spheres_png{"spheres" + std::to_string(i + cameras.size()) + ".png"};
        pixels.save_png(spheres_png);
        std::cout << "Wrote to: " << spheres_png << "\n";
    }
}

void double_spheres(const Camera& camera, const Sphere& sphere, const Sphere& ground, Pixels& pixels) {
    for (int row = 0; row < pixels.rows; ++row) {
        double v = static_cast<double>(row) / (pixels.rows - 1);
        for (int col = 0; col < pixels.columns; ++col) {
            double u = static_cast<double>(col) / (pixels.columns - 1);
            Ray ray = camera.compute_ray(u, v);

            // hitting the ground
            std::optional<double> time_ground = ground.intersect_geo(ray);
            if(time_ground.has_value()) {
                Hit hit = ground.construct_hit(ray, time_ground.value());
                pixels(row, col) = {v, v, v}; // {std::abs(hit.normal.x), std::abs(hit.normal.y), std::abs(hit.normal.z)};
            }
            else {
                pixels(row, col) = Black; // default background to Black
            }

            // hitting the sphere
            std::optional<double> time = sphere.intersect_geo(ray);
            if (time.has_value()) {
                Hit hit = sphere.construct_hit(ray, time.value());
                pixels(row, col) = hit.normal;//{std::abs(hit.normal.z), std::abs(hit.normal.x), std::abs(hit.normal.y)};
            }
        }
    }
}