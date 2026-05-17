#include "scene.h"

#include <iostream>

#include "mesh/obj_loader.h"

bool Scene::loadFromOBJ(const std::string& path)
{
    if (!LoadOBJ(path, mesh))
    {
        std::cout << "OBJ load failed\n";
        return false;
    }

    std::cout << "Vertices: " << mesh.vertices.size() << std::endl;
    std::cout << "Indices: " << mesh.indices.size() << std::endl;

    hemesh.buildFromMesh(mesh);
    hemesh.updateVertexNormals();

    mesh = hemesh.toMesh();

    centerMesh();

    return true;
}

void Scene::centerMesh()
{
    if (mesh.vertices.empty())
        return;

    Eigen::Vector3f center(0, 0, 0);

    for (const auto& v : mesh.vertices)
    {
        center += v.position;
    }

    center /= static_cast<float>(mesh.vertices.size());

    for (auto& v : mesh.vertices)
    {
        v.position -= center;
    }
}