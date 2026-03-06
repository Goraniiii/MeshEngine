#pragma once

#include "../mesh/mesh.h"
#include <glad/glad.h>

class Renderer
{
public:

    void uploadMesh(const Mesh& mesh);
    void draw();

private:

    unsigned int VAO = 0;
    unsigned int VBO = 0;
    unsigned int EBO = 0;

    int indexCount = 0;
};