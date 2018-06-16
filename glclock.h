#ifndef GLCLOCK_H
#define GLCLOCK_H

#include "clock.h"
#include "glclockwindow.h"

#include <QObject>
#include <QOpenGLWindow>

class GLClock : public Clock
{
    Q_OBJECT

public:
    explicit GLClock(QObject *parent = Q_NULLPTR);
    ~GLClock();

signals:
    void timeout();

protected:
    virtual void updateDisplay() Q_DECL_OVERRIDE;

private:
    GLClockWindow *window;
};

#endif // GLCLOCK_H
