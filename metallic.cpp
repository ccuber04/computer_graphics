#include "metallic.h"
#include "ray.h"
#include "hit.h"
#include "random.h"

#include <algorithm>

Metallic::Metallic(Color color, bool emitting, double fuzz)
    : Material{"Matallic", color, emitting}, fuzz{std::clamp(fuzz, 0.0, 1.0)}{}

Ray Metallic::scatter(const Ray &ray, const Hit &hit) const {
    Vector3D scattered = reflect(ray.direction, hit.normal);
    scattered = scattered + (fuzz*random_hemisphere(hit.normal));
    return Ray{hit.position, unit(scattered)};
}