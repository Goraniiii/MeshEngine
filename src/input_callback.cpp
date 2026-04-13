#include "input_callback.h"

InputCallback::InputCallback(Renderer* renderer, HEMesh* hemesh, Mesh* mesh) 
: m_renderer(renderer), m_hemesh(hemesh), m_mesh(mesh) {}

void InputCallback::updateVisuals() {
    *m_mesh = m_hemesh->toMesh();
    m_renderer->uploadMesh(*m_mesh);
}

void InputCallback::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    auto* handler = static_cast<InputCallback*>(glfwGetWindowUserPointer(window));
    if (handler) handler->handleMouseButton(window, button, action, mods);
}

void InputCallback::cursorPosCallback(GLFWwindow* window, double x, double y) {
    auto* handler = static_cast<InputCallback*>(glfwGetWindowUserPointer(window));
    if (handler) handler->handleCursorPos(x, y);
}

void InputCallback::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    auto* handler = static_cast<InputCallback*>(glfwGetWindowUserPointer(window));
    if (handler) handler->handleScroll(yoffset);
}

void InputCallback::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    auto* handler = static_cast<InputCallback*>(glfwGetWindowUserPointer(window));
    if (handler) handler->handleKey(key, action);
}

// ------------------------------

void InputCallback::handleMouseButton(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            m_dragging = true;
            glfwGetCursorPos(window, &m_lastX, &m_lastY);
        } else if (action == GLFW_RELEASE) {
            m_dragging = false;
        }
    }
}

void InputCallback::handleCursorPos(double x, double y) {
    if (!m_dragging) return;

    float dx = static_cast<float>(x - m_lastX);
    float dy = static_cast<float>(y - m_lastY);
    float sensitivity = 0.005f;

    m_renderer->camera.yaw += dx * sensitivity;
    m_renderer->camera.pitch += dy * sensitivity;

    // Pitch Clamp
    if (m_renderer->camera.pitch > 1.5f) m_renderer->camera.pitch = 1.5f;
    if (m_renderer->camera.pitch < -1.5f) m_renderer->camera.pitch = -1.5f;

    m_lastX = x;
    m_lastY = y;
}

void InputCallback::handleScroll(double yoffset) {
    m_renderer->camera.distance -= static_cast<float>(yoffset) * 0.5f;
    if (m_renderer->camera.distance < 1.0f) m_renderer->camera.distance = 1.0f;
}

void InputCallback::handleKey(int key, int action) {
    if (action != GLFW_PRESS) return;

    switch (key) {
        case GLFW_KEY_W:
            m_renderer->wireframe = !m_renderer->wireframe;
            break;

        case GLFW_KEY_S: // Smoothing Laplacian
            m_hemesh->smoothLaplacian(0.1f, 1);
            updateVisuals();
            break;

        case GLFW_KEY_T: // Smoothing Taubin
            m_hemesh->smoothTaubin(0.1f, -0.11f, 1);
            updateVisuals();
            break;

        case GLFW_KEY_A: // Adaptive Smoothing
            m_hemesh->smoothAdaptive(1.0f, 10.0f);
            updateVisuals();
            break;

        case GLFW_KEY_C: // Curvature Visualization Toggle
            m_renderer->useColor = !m_renderer->useColor;
            if (m_renderer->useColor) {
                m_hemesh->computeGaussianCurvature();
                updateVisuals();
            }
            break;
    }
}