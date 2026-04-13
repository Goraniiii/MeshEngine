#include "common.h"
#include <iostream>

#include "mesh/obj_loader.h"
#include "viewer/renderer.h"
#include "mesh/half_edge.h"
#include "input_callback.h"

std::string objFilePath = "C:\\Users\\gony4\\source\\repos\\MeshEngine\\assets\\mesh\\teapot.obj";

double lastX = 0;
double lastY = 0;
bool dragging = false;

void centerMesh(Mesh& mesh);

int main()
{
    if (!glfwInit()) {
        std::cout << "GLFW init failed\n";
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(800, 600, "MeshEngine", NULL, NULL);

    if (!window) {
        std::cout << "Window creation failed\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "GLAD init failed\n";
        return -1;
    }

    // -----------------------
    // Mesh load
    // -----------------------
    Mesh mesh;
    LoadOBJ(objFilePath, mesh);

    std::cout << "Vertices: " << mesh.vertices.size() << std::endl;
    std::cout << "Indices: " << mesh.indices.size() << std::endl;

    // -----------------------
    // Half Edge mesh
    // -----------------------
    HEMesh hemesh;
    hemesh.buildFromMesh(mesh);
    hemesh.updateVertexNormals();
    mesh = hemesh.toMesh();

    // -----------------------
    // Center mesh
    // -----------------------

    centerMesh(mesh);

    // -----------------------
    // Renderer
    // -----------------------
    Renderer renderer;
    renderer.init();
    renderer.uploadMesh(mesh);

    // -----------------------
    // Input callbacks
    // -----------------------
    InputCallback inputHandler(&renderer, &hemesh, &mesh);

    glfwSetWindowUserPointer(window, &inputHandler);

    glfwSetMouseButtonCallback(window, InputCallback::mouseButtonCallback);
    glfwSetCursorPosCallback(window, InputCallback::cursorPosCallback);
    glfwSetScrollCallback(window, InputCallback::scrollCallback);
    glfwSetKeyCallback(window, InputCallback::keyCallback);

    // -----------------------
    // Render loop
    // -----------------------

    while (!glfwWindowShouldClose(window))
    {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        glViewport(0, 0, width, height);

        renderer.draw(width, height);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}


void centerMesh(Mesh& mesh) {
    if (mesh.vertices.empty()) return;

    Eigen::Vector3f center(0, 0, 0);
    for (const auto& v : mesh.vertices) {
        center += v.position;
    }
    center /= (float)mesh.vertices.size();

    for (auto& v : mesh.vertices) {
        v.position -= center;
    }
}
