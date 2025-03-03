#pragma once

#include "color.h"

#include <string>

class Ray;
class Hit;

class Material {
public:
    Material(std::string name, Color color, bool emitting);

    virtual Ray scatter(const Ray& ray, const Hit& hit) const = 0;
    virtual ~Material() = default;

    std::string name;
    Color color;
    bool emitting;
};

Vector3D reflect(const Vector3D& v, const Vector3D& normal);
Vector3D refract(const Vector3D& v, const Vector3D& normal, double index_ratio);
double schlick(double cos_theta, double index_ratio);