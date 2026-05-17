#pragma once

#include "mesh/mesh.h"
#include "mesh/half_edge.h"

class Scene
{
public:
    Mesh mesh;
    HEMesh hemesh;

public:
    bool loadFromOBJ(const std::string& path);

private:
    void centerMesh();
};