#include "glclock.h"

GLClock::GLClock(QObject *parent)
    : Clock(parent),
      window(new GLClockWindow(this))
{
    this->window->show();

    connect(this->window, &QOpenGLWindow::frameSwapped, this, &GLClock::timeout);
}

GLClock::~GLClock()
{
    delete this->window;
}

void GLClock::updateDisplay()
{
    this->window->update();
}
