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

void Renderer::draw()
{
    glBindVertexArray(VAO);

    glDrawElements(
        GL_TRIANGLES,
        indexCount,
        GL_UNSIGNED_INT,
        0
    );
}