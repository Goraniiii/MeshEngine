#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "mesh/obj_loader.h"
#include "viewer/renderer.h"

Renderer* gRenderer = nullptr;

double lastX = 0;
double lastY = 0;
bool dragging = false;

void mouseButton(GLFWwindow* window, int button, int action, int mods);
void cursorPos(GLFWwindow* window, double x, double y);
void scroll(GLFWwindow* window, double xoffset, double yoffset);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

int main()
{
    if (!glfwInit())
    {
        std::cout << "GLFW init failed\n";
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(800, 600, "MeshEngine", NULL, NULL);

    if (!window)
    {
        std::cout << "Window creation failed\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "GLAD init failed\n";
        return -1;
    }

    // -----------------------
    // Mesh load
    // -----------------------

    Mesh mesh;

    LoadOBJ(
        "C:\\Users\\gony4\\source\\repos\\MeshEngine\\assets\\mesh\\teapot.obj",
        mesh
    );

    std::cout << "Vertices: " << mesh.vertices.size() << std::endl;
    std::cout << "Indices: " << mesh.indices.size() << std::endl;

    // -----------------------
    // Center mesh
    // -----------------------

    Eigen::Vector3f center(0, 0, 0);

    for (const auto& v : mesh.vertices)
    {
        center += v.position;
    }

    center /= mesh.vertices.size();

    for (auto& v : mesh.vertices)
    {
        v.position -= center;
    }

    // -----------------------
    // Renderer
    // -----------------------

    Renderer renderer;

    renderer.init();
    renderer.uploadMesh(mesh);

    gRenderer = &renderer;

    // -----------------------
    // Input callbacks
    // -----------------------

    glfwSetMouseButtonCallback(window, mouseButton);
    glfwSetCursorPosCallback(window, cursorPos);
    glfwSetScrollCallback(window, scroll);
    glfwSetKeyCallback(window, keyCallback);

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

void mouseButton(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        if (action == GLFW_PRESS)
        {
            dragging = true;
            glfwGetCursorPos(window, &lastX, &lastY);
        }
        else if (action == GLFW_RELEASE)
        {
            dragging = false;
        }
    }
}

void cursorPos(GLFWwindow* window, double x, double y)
{
    if (!dragging)
        return;

    float dx = x - lastX;
    float dy = y - lastY;

    float sensitivity = 0.005f;

    gRenderer->camera.yaw += dx * sensitivity;
    gRenderer->camera.pitch += dy * sensitivity;

    if (gRenderer->camera.pitch > 1.5f)
        gRenderer->camera.pitch = 1.5f;

    if (gRenderer->camera.pitch < -1.5f)
        gRenderer->camera.pitch = -1.5f;

    lastX = x;
    lastY = y;
}

void scroll(GLFWwindow* window, double xoffset, double yoffset)
{
    gRenderer->camera.distance -= yoffset * 0.5f;

    if (gRenderer->camera.distance < 1.0f)
        gRenderer->camera.distance = 1.0f;
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        if (key == GLFW_KEY_W)
        {
            gRenderer->wireframe = !gRenderer->wireframe;
        }
    }
}