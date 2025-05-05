#include "subsurface.h"
#include "ray.h"
#include "hit.h"
#include "random.h"
#include "object.h"
#include <cmath>

SubsurfaceScattering::SubsurfaceScattering(const Texture *texture, bool emitting, double density)
: Material{texture, emitting} {
    Color sum{0, 0, 0};
    int samples{20};
    for (int i = 0; i < samples; ++i) {
        sum += texture->value(random_double(), random_double());
    }
    Color albedo = sum / static_cast<double>(samples);
    scattering = density * albedo;
    absorption = density * (Color{1, 1, 1} - albedo);
    extinction = scattering + absorption;
}

Ray SubsurfaceScattering::scatter(const Ray& ray, const Hit& hit) const {
    double extinction_avg = (extinction.x + extinction.y + extinction.z) / 3.0;

    if (extinction_avg == 0) {
        return Ray{hit.position, unit(random_unit_vector() + hit.normal)};
    }

//    if (random_double() < 0.05) {
//        Vector3D reflected = reflect(ray.direction, hit.normal);
//        return Ray{hit.position, reflected};
//    }

    Point3D current_pos = hit.position;
    Vector3D current_dir = ray.direction;

    for (int i = 0; i < 10; ++i) {
        double distance = -std::log(1.0 - random_double()) / extinction_avg;
        Vector3D direction = random_hemisphere(-current_dir);
        Point3D new_pos = current_pos + distance * direction;

        Ray new_ray{new_pos, direction};
        std::optional<double> time = hit.object->intersect(new_ray);
        if (!time.has_value() || *time > distance) {
            return new_ray;
        }

        current_pos = new_pos;
        current_dir = direction;
    }
    Ray new_ray{current_pos, current_dir};
    return new_ray;
}
