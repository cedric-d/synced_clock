#include "clock.h"

// Interval between rate updates (in ms)
#define RATE_UPDATE_RATE 500

Clock::Clock(QObject *parent)
    : QObject(parent),
      counter(0)
{
    this->rate.last_counter = 0;
    this->rate.last_date.start();
    this->rate.current = 0;
}

void Clock::nextTick()
{
    this->previous = this->now;
    this->now = QTime::currentTime();
    this->counter++;

    if (this->rate.last_date.hasExpired(RATE_UPDATE_RATE)) {
        const quint64 nbticks = this->counter - this->rate.last_counter;
        const qint64 duration = this->rate.last_date.nsecsElapsed();

        this->rate.current = nbticks / (duration / 1000000000.0);

        this->rate.last_counter = this->counter;
        this->rate.last_date.restart();
    }

    this->updateDisplay();
}
