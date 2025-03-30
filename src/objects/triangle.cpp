#include "triangle.h"
#include "ray.h"
#include "constants.h"

Triangle::Triangle(const Point3D& vertex0, const Point3D& vertex1, const Point3D& vertex2,
         const Material& material)
         :Object{material}, vertex0{vertex0}, vertex1{vertex1}, vertex2{vertex2},
         edge1{vertex1 - vertex0}, edge2{vertex2 - vertex0},
         normal{unit(cross(edge1, edge2))}{}

std::optional<double> Triangle::intersect(const Ray& ray) const {
    // Moeller-Trumbore triangle intersection
    Vector3D h = cross(ray.direction, edge2);
    double a = dot(edge1, h);
    if (a > -Constants::epsilon && a < Constants::epsilon) {
        // ray is parallel to the triangle
        return {};
    }

    double f = 1.0 / a;
    Vector3D s = ray.origin - vertex0;
    double u = f * dot(s, h);
    if (u < 0.0 || u > 1.0) {
        // ray is outside of u coords of [0,1]
        return {};
    }

    Vector3D q = cross(s, edge1);
    double v = f * dot(ray.direction, q);
    if (v < 0.0 || v > 1.0) {
        // ray is outside of v coords [0, 1]
        return {};
    }

    double t = f * dot(edge2, q);
    if (t > Constants::epsilon) {
        return t;
    }
    else {
        return {}; // ray origin too close to triangle surface
    }

}
// Hit Triangle::construct_hit(const Ray& ray, double time) const override;
// std::pair<double, double> Triangle::uv(const Hit& hit) const override;