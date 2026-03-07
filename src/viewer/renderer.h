#pragma once

#include "../mesh/mesh.h"
#include "camera.h"
#include <glad/glad.h>

class Renderer
{
public:

    void uploadMesh(const Mesh& mesh);
    void draw(int width, int height);

    void initShader();

    Camera camera;

private:

    GLuint VAO;
    GLuint VBO;
    GLuint EBO;

    GLuint shaderProgram;

    GLuint mvpLocation;

    int indexCount;
};