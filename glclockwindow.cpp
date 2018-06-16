#include "clock.h"
#include "glclockwindow.h"

#include <QOpenGLFunctions>
#include <QPainter>
#include <QSurfaceFormat>

GLClockWindow::GLClockWindow(const Clock *clock, QWindow *parent)
    : QOpenGLWindow(NoPartialUpdate, parent),
      clock(clock)
{
    QSurfaceFormat format;
    format.setSwapInterval(1);
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    this->setFormat(format);

    this->resize(640, 320);
    this->show();
}

void GLClockWindow::paintGL()
{
    QOpenGLFunctions *f = this->context()->functions();

    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QPainter painter(this);
    QFont font = painter.font();

    font.setPointSize(28);
    painter.setFont(font);
    painter.setPen(Qt::lightGray);
    painter.drawText(0,   0, 240, 60, Qt::AlignRight|Qt::AlignVCenter, tr("Current:"));
    painter.drawText(0,  60, 240, 60, Qt::AlignRight|Qt::AlignVCenter, tr("Previous:"));
    painter.drawText(0, 120, 240, 60, Qt::AlignRight|Qt::AlignVCenter, tr("Interval:"));
    painter.drawText(0, 180, 240, 60, Qt::AlignRight|Qt::AlignVCenter, tr("Counter:"));
    painter.drawText(0, 240, 240, 60, Qt::AlignRight|Qt::AlignVCenter, tr("Rate:"));

    font.setPointSize(32);
    painter.setFont(font);
    painter.setPen(Qt::white);

    const QTime &now = this->clock->getNow();
    const QTime &prev = this->clock->getPrevious();

    painter.drawText(240,   0, 400, 60, Qt::AlignRight|Qt::AlignVCenter,
                     now.toString("HH:mm:ss.zzz"));
    painter.drawText(240,  60, 400, 60, Qt::AlignRight|Qt::AlignVCenter,
                     prev.toString("HH:mm:ss.zzz"));
    painter.drawText(240, 120, 400, 60, Qt::AlignRight|Qt::AlignVCenter,
                     tr("%L1 ms").arg(prev.msecsTo(now)));
    painter.drawText(240, 180, 400, 60, Qt::AlignRight|Qt::AlignVCenter,
                     QString::number(this->clock->getCounter()));
    painter.drawText(240, 240, 400, 60, Qt::AlignRight|Qt::AlignVCenter,
                     tr("%L1 Hz").arg(this->clock->getRate(), 0, 'f', 1));
}
