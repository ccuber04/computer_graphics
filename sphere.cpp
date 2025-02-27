#include "sphere.h"
#include "constants.h"
#include <cmath>

Sphere::Sphere(const Point3D& center, double radius, const Material* material)
        :center{center}, radius{radius}, material{material} {
    if (radius < 0) {
        throw std::runtime_error("Radius cannot be negative");
    }
}

Hit Sphere::construct_hit(const Ray& ray, double time) const {
    // calculate the surface normal
    Point3D point = ray.at(time);
    Vector3D normal = (point - center) / radius;
    return Hit{time, point, normal, this};
}

std::optional<double> Sphere::intersect_alg(const Ray& ray) const { // algebraic intersection
    Vector3D center_pos = ray.origin - center;
    double a = dot(ray.direction, ray.direction);
    double b = 2 * dot(ray.direction, center_pos);
    double c = dot(center_pos, center_pos) - radius*radius;
    double d = b*b - 4*a*c; // discriminant

    // check if the ray misses
    if (d < -8*Constants::epsilon) {
        return std::nullopt;
    }
    else if (std::abs(d) < 8*Constants::epsilon) {
        return -b / (2*a); // single hit return time
    }

    double t_neg = (-b - sqrt(d)) / (2*a);
    double t_pos = (-b + sqrt(d)) / (2*a);
    if (t_neg > Constants::epsilon) {
        return t_neg;
    }
    else {
        return t_pos;
    }
}

std::optional<double> Sphere::intersect_geo(const Ray& ray) const{ // geometric intersection
    Vector3D center_pos =  center - ray.origin;
    double cp_length = dot(center_pos, center_pos);
    double tmp = dot(center_pos, ray.direction);
    double q_square = cp_length - tmp*tmp;
    double r_square = radius*radius;

    // check if the ray misses
    if (q_square > r_square + 2*Constants::epsilon) {
        return std::nullopt;
    }
    else if (std::abs(q_square - r_square) < 2*Constants::epsilon) {
        return tmp;
    }

    double h = sqrt(r_square - q_square);
    double t_neg = tmp - h;
    double t_pos = tmp + h;
    if (t_neg > Constants::epsilon) {
        return t_neg;
    }
    else if (t_pos > Constants::epsilon) {
        return t_pos;
    }
    return {};
}

std::ostream& operator<<(std::ostream& os, const Sphere& sphere) {
    return os << "Sphere(center=" << sphere.center << ", radius=" << sphere.radius << ")\n";
}

