#pragma once

#include "point3d.h"
#include "hit.h"
#include "object.h"
#include <optional>

class Ray;
class Material;

class Sphere : public Object {
public:
    Sphere(const Point3D& center, double radius, const Material* material=nullptr);

    // returns the distance if the ray intersects this sphere, otherwise std::nullopt
    std::optional<double> intersect(const Ray& ray) const override;
    std::optional<double> intersect2(const Ray& ray) const;
    std::optional<double> intersect_algebraic(const Ray& ray) const;
    Hit construct_hit(const Ray& ray, double time) const override;
    
    Point3D center;
    double radius;
};

