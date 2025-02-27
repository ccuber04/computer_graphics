#pragma once

#include "point3d.h"
#include "hit.h"
#include "ray.h"
#include <optional>

class Material;

class Sphere {
public:
    Sphere(const Point3D& center, double radius, const Material* material=nullptr);
    Hit construct_hit(const Ray& ray, double time) const;

    std::optional<double> intersect_alg(const Ray& ray) const; // algebraic
    std::optional<double> intersect_geo(const Ray& ray) const; // geometric


    Point3D center;
    double radius;

    const Material* material;
};

std::ostream& operator<<(std::ostream& os, const Sphere& sphere);