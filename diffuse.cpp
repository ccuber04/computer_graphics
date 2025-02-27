#include "diffuse.h"
#include "random.h"
#include "hit.h"
#include "ray.h"

Diffuse::Diffuse(Color color, bool emitting)
    : Material{"Diffuse", color, emitting}{}

Ray Diffuse::scatter(const Ray& ray, const Hit& hit) const {
    Vector3D scattered = random_hemisphere(hit.normal);
//    Vector3D scattered = reflect(ray.direction, hit.normal); // specular
    return Ray{hit.position, scattered};
}