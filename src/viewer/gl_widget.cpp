#include "gl_widget.h"

#include <iostream>

#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>

#include <glad/glad.h>

GLWidget::GLWidget(QWidget* parent)
    : QOpenGLWidget(parent)
{
}

void GLWidget::initializeGL()
{
    if (!gladLoadGL())
    {
        std::cout << "GLAD init failed\n";
        return;
    }

    glEnable(GL_DEPTH_TEST);

    m_scene.loadFromOBJ(
        "C:/Users/gony4/source/repos/MeshEngine/assets/mesh/teapot.obj"
    );

    m_renderer.init();

    m_renderer.uploadMesh(m_scene.mesh);
}

void GLWidget::paintGL()
{
    int width = this->width();
    int height = this->height();

    glViewport(0, 0, width, height);

    m_renderer.draw(width, height);

    update();
}

void GLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void GLWidget::mousePressEvent(QMouseEvent* event)
{
    m_lastMousePos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent* event)
{
    QPoint delta = event->pos() - m_lastMousePos;

    m_lastMousePos = event->pos();

    // TODO:
    // camera rotate
}

void GLWidget::wheelEvent(QWheelEvent* event)
{
    // TODO:
    // zoom
}

void GLWidget::keyPressEvent(QKeyEvent* event)
{
    switch (event->key())
    {
    case Qt::Key_W:
        std::cout << "W pressed\n";
        break;

    case Qt::Key_S:
        std::cout << "S pressed\n";
        break;
    }
}