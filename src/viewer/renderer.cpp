#include "renderer.h"

void Renderer::uploadMesh(const Mesh& mesh)
{
    indexCount = mesh.indices.size();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        mesh.vertices.size() * sizeof(Eigen::Vector3f),
        mesh.vertices.data(),
        GL_STATIC_DRAW
    );

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        mesh.indices.size() * sizeof(unsigned int),
        mesh.indices.data(),
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Eigen::Vector3f),
        (void*)0
    );

    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void Renderer::draw(int width, int height)
{
    glUseProgram(shaderProgram);

    if (wireframe)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    float aspect = (float)width / height;

    Eigen::Matrix4f view = camera.getViewMatrix();
    Eigen::Matrix4f proj = camera.getProjectionMatrix(aspect);

    Eigen::Matrix4f MVP = proj * view;


    glUniformMatrix4fv(
        mvpLocation,
        1,
        GL_FALSE,
        MVP.data()
    );

    glBindVertexArray(VAO);

    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
}

void Renderer::initShader()
{
    const char* vsSrc = R"(

    #version 330 core
    layout(location=0) in vec3 position;

    uniform mat4 MVP;

    void main()
    {
        gl_Position = MVP * vec4(position,1.0);
    }

    )";

    const char* fsSrc = R"(

    #version 330 core
    out vec4 FragColor;

    void main()
    {
        FragColor = vec4(0.8,0.8,0.8,1.0);
    }

    )";

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vsSrc, nullptr);
    glCompileShader(vs);

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fsSrc, nullptr);
    glCompileShader(fs);

    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vs);
    glAttachShader(shaderProgram, fs);
    glLinkProgram(shaderProgram);

    glDeleteShader(vs);
    glDeleteShader(fs);

    mvpLocation = glGetUniformLocation(shaderProgram, "MVP");
}