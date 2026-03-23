#include "renderer.h"

#include "shader.h"
std::unique_ptr<Shader> meshShader;

void Renderer::init() {
    meshShader = std::make_unique<Shader>("C:\\Users\\gony4\\source\\repos\\MeshEngine\\shaders\\curvature.vert", "C:\\Users\\gony4\\source\\repos\\MeshEngine\\shaders\\curvature.frag");

    glEnable(GL_DEPTH_TEST);
}

void Renderer::uploadMesh(const Mesh& mesh)
{
    indexCount = (unsigned int)mesh.indices.size();

    // 기존 VAO/VBO/EBO가 있다면 해제하는 로직이 있으면 좋습니다.
    if (VAO != 0) {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // 1. Vertex Buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(Vertex), mesh.vertices.data(), GL_STATIC_DRAW);

    // 2. Index Buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(unsigned int), mesh.indices.data(), GL_STATIC_DRAW);

    // 3. Attribute 설정 (location 값은 셰이더와 일치해야 함)
    // Position (Location 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);

    // Normal (Location 1)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);

    // Color (Location 2) - 곡률 시각화를 위해 추가
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

    // 셰이더 사용 시작
    meshShader->use();

    float aspect = (float)width / (float)height;
    Eigen::Matrix4f view = camera.getViewMatrix();
    Eigen::Matrix4f proj = camera.getProjectionMatrix(aspect);

    Eigen::Matrix4f MV = view;
    Eigen::Matrix4f MVP = proj * view;
    Eigen::Matrix3f normalMatrix = MV.block<3, 3>(0, 0).inverse().transpose();

    // Shader 클래스의 유틸리티 함수를 사용하여 유니폼 설정
    meshShader->setMat4("MVP", MVP);
    meshShader->setMat4("MV", MV);
    meshShader->setMat3("normalMatrix", normalMatrix);
    meshShader->setBool("wireframe", wireframe);
    meshShader->setBool("useColor", useColor); // 곡률 모드 토글

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}