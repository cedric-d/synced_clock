#ifndef SYNCEDTIMER_H
#define SYNCEDTIMER_H

#include <QObject>
#include <QSocketNotifier>
#include <QThread>


class SyncedTimer;

class SyncedTimerWorker : public QThread
{
    Q_OBJECT

private slots:
    void wakeup(int fd);

signals:
    void timeout();

private:
    SyncedTimerWorker(const int fd);
    ~SyncedTimerWorker();

    QSocketNotifier *notifier;

    friend class SyncedTimer;
};

class SyncedTimer : public QObject
{
    Q_OBJECT

public:
    explicit SyncedTimer(QObject *parent = 0);
    ~SyncedTimer();

    static const qint32 EARLY_WAKEUP;

signals:
    void timeout();

public slots:
    void start(int frequency);
    void stop();

private:
    SyncedTimerWorker *worker;
    int fd;
};

#endif // SYNCEDTIMER_H
