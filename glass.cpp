#include "glass.h"
#include "random.h"
#include "hit.h"
#include "ray.h"

#include <algorithm>

Glass::Glass(Color color, bool emitting)
        : Material{"Glass", color, emitting}{}

Ray Glass::scatter(const Ray& ray, const Hit& hit) const {
    double n1 = 1.0;
    double n2 = 1.5;

    Vector3D normal = hit.normal;
    if (dot(ray.direction, normal) > 0) {
        normal *= -1;
        std::swap(n1, n2);
    }
    double cos_theta = -dot(ray.direction, normal);
    double probability = 0;//schlick(cos_theta, n1/n2);
    Vector3D scattered = random_hemisphere(hit.normal);

    if (random_double() < probability) { // [0, 1) < prob
        Vector3D reflected = reflect(ray.direction, normal);
        return Ray{hit.position, reflected};
    }
    else {
        Vector3D refracted = refract(ray.direction, normal, n1/n2);
        return Ray{hit.position, refracted};
    }
    return Ray{hit.position, scattered};
}