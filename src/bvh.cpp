#include "bvh.h"
#include "constants.h"
#include "random.h"
#include "hit.h"
#include <algorithm>

BVH::BVH(const std::vector<std::unique_ptr<Object>>& objects) {
    // Nodes of the BVH only need a pointer to the object, not to maintain ownership.
    // Build a list of raw object pointers
    std::vector<Object*> object_pointers;
    for (const auto& obj : objects) {
        object_pointers.push_back(obj.get());
    }
    root = build_tree(object_pointers);
}

std::optional<Hit> BVH::find_nearest(const Ray& ray) const {
    // test intersection with the root node
    auto [object, t] = intersect(root, ray);

    // if you hit something, then construct a hit on the object
    if (t.has_value()) {
        return object->construct_hit(ray, *t);
    }
    // otherwise no hit
    else {
        return {};
    }
}

BVH::Node* BVH::build_tree(std::vector<Object*> objects) const {
    // Recursively build a tree
    Node* n = new Node();

    // at leaf nodes, set the node object and bounding box
    if (objects.size() == 1) {
        n->object = objects.front();
        n->box = objects.front()->bounding_box();
    }
    // for all others partition and recursively call build_tree
    else {
        auto [left, right] = partition_along_random_axis(objects);
        n->left = build_tree(left);
        n->right = build_tree(right);

        // make sure to handle surrounding boxes at non-leaf nodes
        n->box = surrounding_box(n->left->box, n->right->box);
    }
    return n;
}


bool BVH::compare_boxes(Object* a, Object* b, int axis) const {
    // For the given axis (x=0, y=1, z=2) returns whether a's box is less than b's
    Vector3D a_min = a->bounding_box().minimum;
    Vector3D b_min = b->bounding_box().minimum;
    if (axis == 0) {
        return a_min.x < b_min.x;
    }
    else if (axis == 1) {
        return a_min.y < b_min.y;
    }
    else if (axis == 2) {
        return a_min.z < b_min.z;
    }
    else {
        throw std::runtime_error("axis " + std::to_string(axis) + " does not exist");
    }
}

std::pair<std::vector<Object*>, std::vector<Object*>> BVH::partition_along_random_axis(std::vector<Object*> objects) const {
    // sort objects along a random axis
    int axis = static_cast<int>(random_double(0, 3));
    std::sort(objects.begin(), objects.end(),
              [=, this](Object* a, Object* b) {
                  return compare_boxes(a, b, axis);
              });

    // divide objects vector into two vectors
    size_t midpoint = objects.size() / 2;
    std::vector<Object*> left(objects.begin(), objects.begin() + midpoint);
    std::vector<Object*> right(objects.begin() + midpoint, objects.end());

    return {left, right};
}

std::pair<Object*, std::optional<double>> BVH::intersect(Node* n, const Ray& ray) const {
    // First handle the ray not intersection the box, then handle leaf nodes
    bool intersected = n->box.intersect(ray);
    if (!intersected) {
        return {nullptr, {}};
    }

    if (!n->left && !n->right) {
        auto t = n->object->intersect(ray);
        return {n->object, t};
    }
    
    // Remember that boxes can overlap, so you must test both left and right boxes.
    auto [left, left_t] = intersect(n->left, ray);
    auto [right, right_t] = intersect(n->right, ray);

    // Given the results of hitting possible left or right or both, return the closest
    if (left_t.has_value() && right_t.has_value()) {
        if (*left_t < *right_t) {
            return {left, left_t};
        }
        return {right, right_t};
    }
    else if (left_t.has_value()) {
        return {left, left_t};
    }
    else {
        return {right, right_t};
    }
}
