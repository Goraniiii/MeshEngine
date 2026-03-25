#pragma once

#include <glad/glad.h>

#include "../mesh/mesh.h"
#include "camera.h"

class Renderer
{
public:

    void init();
    void uploadMesh(const Mesh& mesh);
    void draw(int width, int height);

    bool wireframe = false;
    bool useColor = false;

    Camera camera;

private:

    void initShader();

    GLuint VAO = 0;
    GLuint VBO = 0;
    GLuint EBO = 0;

    GLuint shaderProgram = 0;

    GLuint mvpLocation = -1;
    GLuint mvLocation = -1;
    GLuint normalMatrixLocation = -1;
    GLuint wireframeLocation = -1;

    int indexCount = 0;
};