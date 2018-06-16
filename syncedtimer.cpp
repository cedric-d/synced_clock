#include "syncedtimer.h"

#include <cerrno>
#include <cstring>
#include <unistd.h>
#include <sys/timerfd.h>


const qint32 SyncedTimer::EARLY_WAKEUP = 200000;

SyncedTimer::SyncedTimer(QObject *parent)
    : QObject(parent)
{
    Q_ASSERT(EARLY_WAKEUP < 1000000);

    this->fd = timerfd_create(CLOCK_REALTIME, 0);
    if (this->fd == -1) {
        qFatal(tr("Failed to create timer (code %1)").arg(errno).toLocal8Bit().constData());
    }

    this->worker = new SyncedTimerWorker(this->fd);

    connect(this->worker, &SyncedTimerWorker::timeout,
            this, &SyncedTimer::timeout, Qt::QueuedConnection);

    this->worker->start();
}

SyncedTimer::~SyncedTimer()
{
    this->stop();
    delete this->worker;
    close(this->fd);
}

void SyncedTimer::start(int frequency)
{
    struct itimerspec timer;
    struct timespec now;

    memset(&timer, 0, sizeof(timer));
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_nsec = 1000000000UL / frequency;

    clock_gettime(CLOCK_REALTIME, &now);
    timer.it_value.tv_sec = now.tv_sec + (now.tv_nsec + EARLY_WAKEUP) / 1000000000;
    timer.it_value.tv_nsec = 0;

    if (timerfd_settime(this->fd, TFD_TIMER_ABSTIME, &timer, NULL) == -1) {
        qFatal(tr("Failed to start timer (code %1)").arg(errno).toLocal8Bit().constData());
    }
}

void SyncedTimer::stop()
{
    struct itimerspec timer;

    memset(&timer, 0, sizeof(timer));
    if (timerfd_settime(this->fd, 0, &timer, NULL) == -1) {
        qFatal(tr("Failed to stop timer (code %1)").arg(errno).toLocal8Bit().constData());
    }

    this->worker->quit();
    this->worker->wait();
}

SyncedTimerWorker::SyncedTimerWorker(const int fd)
    : notifier(new QSocketNotifier(fd, QSocketNotifier::Read, this))
{
    connect(this->notifier, &QSocketNotifier::activated, this, &SyncedTimerWorker::wakeup);
}

SyncedTimerWorker::~SyncedTimerWorker()
{
    delete this->notifier;
}

void SyncedTimerWorker::wakeup(int fd)
{
    quint64 expires;
    struct timespec now;

    switch (read(fd, &expires, sizeof(expires))) {
    case -1:
        qFatal(tr("Failed to read timer descriptor (code %1)").arg(errno).toLocal8Bit().constData());
        return;

    case 8:
        break;

    default:
        qFatal(tr("Short read from timer descriptor").toLocal8Bit().constData());
        return;
    }

    do {
        clock_gettime(CLOCK_REALTIME, &now);
    } while ((now.tv_nsec % 1000000) >= (1000000 - SyncedTimer::EARLY_WAKEUP));

    emit this->timeout();

    if (expires > 1) {
        qInfo(tr("Missed %Ln timer expiration(s)", "", expires-1).toLocal8Bit().constData());
    }
}
