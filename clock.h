#ifndef CLOCK_H
#define CLOCK_H

#include <QElapsedTimer>
#include <QObject>
#include <QTime>

class Clock : public QObject
{
    Q_OBJECT
public:
    explicit Clock(QObject *parent = Q_NULLPTR);

signals:

public slots:
    void nextTick();

protected:
    virtual void updateDisplay() = 0;

public:
    quint64 getCounter() const
    { return this->counter; }

    const QTime &getNow() const
    { return this->now; }

    const QTime &getPrevious() const
    { return this->previous; }

    qreal getRate() const
    { return this->rate.current; }

private:
    quint64 counter;
    QTime now;
    QTime previous;

    // rate computation data
    struct {
        quint64 last_counter;
        QElapsedTimer last_date;
        qreal current;
    } rate;
};

#endif // CLOCK_H
