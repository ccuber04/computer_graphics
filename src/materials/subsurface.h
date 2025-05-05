#include "material.h"

class SubsurfaceScattering : public Material {
public:
    SubsurfaceScattering(const Texture *texture, bool emitting, double density);

    Ray scatter(const Ray& ray, const Hit& hit) const override;

    Color scattering; // percent chance to scatter
    Color absorption; // percent chance to absorb
    Color extinction;
};
