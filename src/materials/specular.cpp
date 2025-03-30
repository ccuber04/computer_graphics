#include "specular.h"
#include "ray.h"
#include "hit.h"

Specular::Specular(Color color, bool emitting)
    :Material{color, emitting} {}

Ray Specular::scatter(const Ray& ray, const Hit& hit) const {
    Vector3D reflected = reflect(ray.direction, hit.normal);
    return Ray{hit.position, reflected};
}

