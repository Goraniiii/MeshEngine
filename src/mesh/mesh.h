#pragma once
#include <vector>
#include <Eigen/Dense>

struct Mesh
{
    std::vector<Eigen::Vector3f> vertices;
    std::vector<unsigned int> indices;
};

