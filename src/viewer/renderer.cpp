#include "renderer.h"

void Renderer::init()
{
    initShader();

    glEnable(GL_DEPTH_TEST);
}

void Renderer::uploadMesh(const Mesh& mesh)
{
    indexCount = mesh.indices.size();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        mesh.vertices.size() * sizeof(Vertex),
        mesh.vertices.data(),
        GL_STATIC_DRAW
    );

    // index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        mesh.indices.size() * sizeof(unsigned int),
        mesh.indices.data(),
        GL_STATIC_DRAW
    );

    // position
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        (void*)offsetof(Vertex, position)
    );
    glEnableVertexAttribArray(0);

    // normal
    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        (void*)offsetof(Vertex, normal)
    );
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void Renderer::draw(int width, int height)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (wireframe)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glUseProgram(shaderProgram);

    float aspect = (float)width / height;

    Eigen::Matrix4f view = camera.getViewMatrix();
    Eigen::Matrix4f proj = camera.getProjectionMatrix(aspect);

    Eigen::Matrix4f MV = view;
    Eigen::Matrix4f MVP = proj * view;

    Eigen::Matrix3f normalMatrix = MV.block<3, 3>(0, 0).inverse().transpose();

    glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, MVP.data());
    glUniformMatrix4fv(mvLocation, 1, GL_FALSE, MV.data());
    glUniformMatrix3fv(normalMatrixLocation, 1, GL_FALSE, normalMatrix.data());

    glUniform1i(wireframeLocation, wireframe);

    glBindVertexArray(VAO);

    glDrawElements(
        GL_TRIANGLES,
        indexCount,
        GL_UNSIGNED_INT,
        0
    );

    glBindVertexArray(0);
}

void Renderer::initShader()
{
    const char* vsSrc = R"(

    #version 330 core

    layout(location=0) in vec3 position;
    layout(location=1) in vec3 normal;

    uniform mat4 MVP;
    uniform mat4 MV;
    uniform mat3 normalMatrix;

    out vec3 vNormal;
    out vec3 vPosition;

    void main()
    {
        vec4 viewPos = MV * vec4(position,1.0);

        vPosition = viewPos.xyz;
        vNormal = normalize(normalMatrix * normal);

        gl_Position = MVP * vec4(position,1.0);
    }

    )";

    const char* fsSrc = R"(

    #version 330 core

    in vec3 vNormal;
    in vec3 vPosition;

    out vec4 FragColor;

    uniform bool wireframe;

    void main()
    {
        if(wireframe)
        {
            FragColor = vec4(1,1,1,1);
            return;
        }

        vec3 lightPos = vec3(5,5,5);

        vec3 normal = normalize(vNormal);
        vec3 lightDir = normalize(lightPos - vPosition);

        float diff = max(dot(normal, lightDir), 0.0);

        float ambient = 0.2;

        vec3 color = vec3(0.8) * (ambient + diff);

        FragColor = vec4(color,1.0);
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
    mvLocation = glGetUniformLocation(shaderProgram, "MV");
    normalMatrixLocation = glGetUniformLocation(shaderProgram, "normalMatrix");
    wireframeLocation = glGetUniformLocation(shaderProgram, "wireframe");
}