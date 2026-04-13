#ifndef __INPUT_CALLBACK__
#define __INPUT_CALLBACK__

#include "common.h"
#include "viewer/renderer.h"
#include "mesh/half_edge.h"
#include "mesh/mesh.h"

class InputCallback {
public:
    InputCallback(Renderer* renderer, HEMesh* hemesh, Mesh* mesh);

    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void cursorPosCallback(GLFWwindow* window, double x, double y);
    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

private:
    void handleMouseButton(GLFWwindow* window, int button, int action, int mods);
    void handleCursorPos(double x, double y);
    void handleScroll(double yoffset);
    void handleKey(int key, int action);

    Renderer* m_renderer;
    HEMesh* m_hemesh;
    Mesh* m_mesh;

    double m_lastX = 0;
    double m_lastY = 0;
    bool m_dragging = false;

    void updateVisuals();
};


#endif