#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "mesh/obj_loader.h"
#include "viewer/renderer.h"

int main()
{
    if (!glfwInit())
    {
        std::cout << "GLFW init failed\n";
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(1600, 1200, "MeshEngine", NULL, NULL);

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

    // 이제 OpenGL 사용 가능
    Mesh mesh;
    LoadOBJ("C:\\Users\\gony4\\source\\repos\\MeshEngine\\assets\\mesh\\teapot.obj", mesh);

    std::cout << "Vertices: " << mesh.vertices.size() << std::endl;
    std::cout << "Indices: " << mesh.indices.size() << std::endl;

    Renderer renderer;
    renderer.uploadMesh(mesh);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        renderer.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}