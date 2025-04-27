#pragma once

#include <vector>
#include <memory>
#include "object.h"
#include "aabb.h"

class BVH {
public:
    BVH(const std::vector<std::unique_ptr<Object>>& objects);
    std::optional<Hit> find_nearest(const Ray& ray) const;

private:
    class Node {
    public:
        Node* left;
        Node* right;
        Object* object;
        AABB box;

        Node() : left{nullptr}, right{nullptr}, object{nullptr}, box{{0,0,0}, {0,0,0}} {}
    };


    Node* root;

    Node* build_tree(std::vector<Object*> objects) const;
    bool compare_boxes(Object* a, Object* b, int axis) const;
    std::pair<std::vector<Object*>, std::vector<Object*>> partition_along_random_axis(std::vector<Object*> objects) const;
    
    std::pair<Object*, std::optional<double>> intersect(Node* n, const Ray& ray) const;
};
