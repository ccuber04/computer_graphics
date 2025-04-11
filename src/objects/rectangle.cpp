#include "rectangle.h"
#include "constants.h"
#include "aabb.h"

Rectangle::Rectangle(const Vector3D& vertex0, const Vector3D& vertex1, const Vector3D& vertex2, const Vector3D& vertex3,
                     const Material* material)
    : Object{material},
    triangle1{vertex0, vertex1, vertex3, material},
    triangle2{vertex3, vertex2, vertex1, material} {}

AABB Rectangle::bounding_box() const {
    AABB box1 = triangle1.bounding_box();
    AABB box2 = triangle2.bounding_box();
    return surrounding_box(box1, box2);
}

std::optional<double> Rectangle::intersect(const Ray& ray) const {
    std::optional<double> t1 = triangle1.intersect(ray);
    std::optional<double> t2 = triangle2.intersect(ray);
    if (!t1 && !t2) {
        return {};
    }

    if (t1 && t2) {
        return t1.value() < t2.value() ? t1.value() : t2.value();
    }
    else if (t1) {
        return t1.value();
    }
    else if (t2) {
        return t2.value();
    }
}

Hit Rectangle::construct_hit(const Ray& ray, double time) const {
    std::optional<double> t1 = triangle1.intersect(ray);
    if (t1 && std::abs(t1.value() - time) < Constants::epsilon) {
        return triangle1.construct_hit(ray, time);
    } else {
        return triangle2.construct_hit(ray, time);
    }

}

std::pair<double, double> Rectangle::uv(const Hit& hit) const {
    if (hit.object == &triangle1) {
        return triangle1.uv(hit);
    } else {
        return triangle2.uv(hit);
    }
}