#pragma once
#include <memory>
#include <optional>
#include <vector>

#include "constants.h"
#include "hit.h"
#include "point3d.h"

class Sphere;
class Ray;
class Material;

class World {
public:
    void add(Point3D center, double radius, const Material* material=nullptr);
    std::optional<Hit> find_nearest(const Ray& ray) const;
    std::vector<std::unique_ptr<Sphere>> objects;
};
