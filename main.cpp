#include "glclock.h"
#include "standardclock.h"
#include "syncedtimer.h"

#include <QApplication>
#include <QObject>
#include <QString>
#include <QTimer>

// Default update interval (in ms)
static const int DEFAULT_INTERVAL = 10;


int main(int argc, char *argv[])
{
    int timing = -1;
    bool useGLWindow = false;
    bool useSyncedTimer = false;

    QApplication app(argc, argv);

    // parse command line arguments
    Q_FOREACH(const QString & arg, app.arguments().mid(1)) {
        bool ok;
        uint value = arg.toUInt(&ok);
        const QString &lowerArg = arg.toLower();

        if (lowerArg == "gl") {
            useGLWindow = true;
        } else if (lowerArg == "freq") {
            useSyncedTimer = true;
        } else if (lowerArg == "help" || lowerArg == "--help") {
            qInfo(QObject::tr("Usage: %1 [period (in msecs)]\n"
                              "       %1 freq [frequency (in Hz)]\n"
                              "       %1 gl")
                  .arg(app.applicationName()).toLocal8Bit().constData());
            return 0;
        } else if (ok) {
            timing = value;
        } else {
            qWarning(QObject::tr("Unexpected argument: %1").arg(arg).toLocal8Bit().constData());
        }
    }

    // check mutually exclusive arguments
    if (useGLWindow) {
        if (useSyncedTimer)
            qWarning(QObject::tr("Synchronized timer not available with OpenGL").toLocal8Bit().constData());
        if (timing >= 0)
            qWarning(QObject::tr("Custom interval not available with OpenGL").toLocal8Bit().constData());
    }

    // create the clock
    if (useGLWindow) {
        Clock *clock = new GLClock(&app);

        // OpenGL vertical sync will be used as timer
        QObject::connect(static_cast<GLClock *>(clock), &GLClock::timeout,
                         clock, &Clock::nextTick);
    } else {
        Clock *clock = new StandardClock(&app);

        // create the timer
        if (useSyncedTimer) {
            if (timing < 0)
                timing = (1000 / DEFAULT_INTERVAL);

            SyncedTimer *t = new SyncedTimer(&app);
            QObject::connect(t, &SyncedTimer::timeout, clock, &Clock::nextTick);
            t->start(timing);
        } else {
            if (timing < 0)
                timing = DEFAULT_INTERVAL;

            QTimer *t = new QTimer(&app);
            QObject::connect(t, &QTimer::timeout, clock, &Clock::nextTick);
            t->start(timing);
        }
    }

    return app.exec();
}
