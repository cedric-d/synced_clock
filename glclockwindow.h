#ifndef GLCLOCKWINDOW_H
#define GLCLOCKWINDOW_H

#include <QOpenGLWindow>

class Clock;

class GLClockWindow : public QOpenGLWindow
{
    Q_OBJECT

public:
    explicit GLClockWindow(const Clock *clock, QWindow *parent = Q_NULLPTR);

protected:
    virtual void paintGL() Q_DECL_OVERRIDE;

private:
    const Clock *clock;
};

#endif // GLCLOCKWINDOW_H
