#include "renderer.h"

#include "shader.h"
std::unique_ptr<Shader> meshShader;

void Renderer::init() {
    meshShader = std::make_unique<Shader>("shaders\\curvature.vert", "shaders\\curvature.frag");

    glEnable(GL_DEPTH_TEST);
}

void Renderer::uploadMesh(const Mesh& mesh)
{
    indexCount = (uint32_t)mesh.indices.size();

    if (VAO != 0) {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // 1. Vertex Buffer
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(Vertex), mesh.vertices.data(), GL_STATIC_DRAW);

    // 2. Index Buffer
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(uint32_t), mesh.indices.data(), GL_STATIC_DRAW);

    // 3. Attribute
    // Position (Location 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);

    // Normal (Location 1)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);

    // Color (Location 2)
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void Renderer::draw(int width, int height)
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (wireframe)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    meshShader->use();

    float aspect = (float)width / (float)height;
    Eigen::Matrix4f view = camera.getViewMatrix();
    Eigen::Matrix4f proj = camera.getProjectionMatrix(aspect);

    Eigen::Matrix4f MV = view;
    Eigen::Matrix4f MVP = proj * view;
    Eigen::Matrix3f normalMatrix = MV.block<3, 3>(0, 0).inverse().transpose();

    // Shader
    meshShader->setMat4("MVP", MVP);
    meshShader->setMat4("MV", MV);
    meshShader->setMat3("normalMatrix", normalMatrix);
    meshShader->setBool("wireframe", wireframe);
    meshShader->setBool("useColor", useColor);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}