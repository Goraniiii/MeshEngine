#pragma once

#include <glad/glad.h>

#include <QOpenGLWidget>

#include "viewer/renderer.h"
#include "core/scene.h"

class GLWidget : public QOpenGLWidget
{
public:
    GLWidget(QWidget* parent = nullptr);

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

private:
    Renderer m_renderer;
    Scene m_scene;

    QPoint m_lastMousePos;
};