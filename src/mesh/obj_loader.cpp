#include "obj_loader.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include <iostream>

bool LoadOBJ(const std::string& filename, Mesh& mesh)
{
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string warn, err;

    bool ret = tinyobj::LoadObj(
        &attrib,
        &shapes,
        &materials,
        &warn,
        &err,
        filename.c_str()
    );

    if (!warn.empty()) std::cout << warn << std::endl;
    if (!err.empty()) std::cerr << err << std::endl;

    if (!ret) return false;

    for (const auto& shape : shapes)
    {
        for (const auto& index : shape.mesh.indices)
        {
            Eigen::Vector3f v;

            v.x() = attrib.vertices[3 * index.vertex_index + 0];
            v.y() = attrib.vertices[3 * index.vertex_index + 1];
            v.z() = attrib.vertices[3 * index.vertex_index + 2];

            mesh.vertices.push_back(v);
            mesh.indices.push_back(mesh.indices.size());
        }
    }

    return true;
}