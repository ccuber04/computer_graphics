#include "camera.h"
#include "constants.h"
#include <cmath>

Camera::Camera(Point3D position, Point3D target, Vector3D up, double fov, double aspect)
    :position{position} {
    double focal_length = length(position - target);
    double theta = fov * (Constants::pi / 180);
    double width = 2*std::tan(0.5*theta)*focal_length;
    double height = width/aspect;
    Vector3D w = unit(position - target);
    Vector3D u = unit(cross(up, w));
    Vector3D v = cross(w, u);
    horizontal = width*u;
    vertical = height*v;
    upper_left_corner = position - focal_length*w - 0.5*(horizontal - vertical);
}

Ray Camera::compute_ray(double s, double t) const {
    Point3D point = upper_left_corner + s*horizontal - t*vertical;
    Vector3D direction = unit(point - position);
    return Ray{position, direction};
}