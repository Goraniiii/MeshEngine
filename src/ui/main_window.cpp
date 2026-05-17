#include "main_window.h"
#include "../viewer/gl_widget.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    setCentralWidget(new GLWidget(this));
    resize(1280, 720);
}