#pragma once

#include "material.h"
#include "color.h"

class Ray;
class Hit;

class Metallic : public Material {
public:
    Metallic(Color color, bool emitting, double fuzz);

    Ray scatter(const Ray& ray, const Hit& hit) const override;

private:
    double fuzz;
};
