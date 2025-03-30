#include "lambertian.h"
#include "random.h"
#include "hit.h"
#include "ray.h"

Lambertian::Lambertian(Color color, bool emitting)
    :Material{color, emitting} {}

Ray Lambertian::scatter(const Ray&, const Hit& hit) const {
    Vector3D scattered = unit(random_unit_vector() + hit.normal);
    return Ray{hit.position, scattered};
}
