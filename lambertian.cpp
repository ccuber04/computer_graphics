#include "lambertian.h"
#include "random.h"
#include "hit.h"
#include "ray.h"

Lambertian::Lambertian(Color color, bool emitting)
    : Material{"Lambertian", color, emitting}{}

Ray Lambertian::scatter(const Ray& ray, const Hit& hit) const {
    Vector3D scattered = hit.normal + random_hemisphere(hit.normal);
    return Ray{hit.position, scattered};
}