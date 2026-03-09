#pragma once

#include <vector>
#include <Eigen/Dense>

struct Vertex
{
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    Eigen::Vector3f position;
    Eigen::Vector3f normal;

    Vertex() {}

    Vertex(const Eigen::Vector3f& p, const Eigen::Vector3f& n)
        : position(p), normal(n) {
    }
};

struct Mesh
{
    std::vector<Vertex, Eigen::aligned_allocator<Vertex>> vertices;
    std::vector<unsigned int> indices;
};