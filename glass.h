#pragma once

#include "material.h"

class Glass : public Material {
public:
    Glass(Color color, bool emitting); // hardcode name in constructor

    Ray scatter(const Ray& ray, const Hit& hit) const override;
};