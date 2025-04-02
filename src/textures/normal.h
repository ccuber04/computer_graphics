#pragma once

#include "texture.h"

class Normal : public Texture {
public:
    Normal();
    Color value(double u, double v) const override;
};