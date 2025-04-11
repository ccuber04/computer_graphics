#pragma once

#include "object.h"
#include "point3d.h"
#include "vector3d.h"
#include "hit.h"
#include "triangle.h"

class Rectangle : public Object {
public:
    Rectangle(const Vector3D& vertex0, const Vector3D& vertex1, const Vector3D& vertex2, const Vector3D& vertex3,
               const Material* material);

    AABB bounding_box() const override;
    std::optional<double> intersect(const Ray& ray) const override;
    Hit construct_hit(const Ray& ray, double time) const override;
    std::pair<double, double> uv(const Hit& hit) const override;

    Point3D top_left;
    double length, width;

private:
    Triangle triangle1, triangle2;
};