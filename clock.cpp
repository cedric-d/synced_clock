#include "clock.h"

#define RATE_UPDATE_RATE 500

Clock::Clock(QObject *parent)
    : QObject(parent),
      counter(0)
{
    this->rate.counter = 0;
    this->rate.current = 0;
    this->rate.date.start();
}

void Clock::nextTick()
{
    this->previous = this->now;
    this->now = QTime::currentTime();
    this->counter++;

    if (this->rate.date.elapsed() >= RATE_UPDATE_RATE) {
        const quint64 diff = this->counter - this->rate.counter;
        const int elapsed = this->rate.date.restart();

        this->rate.current = (qreal)(diff * 1000) / elapsed;

        this->rate.counter = this->counter;
    }

    this->updateDisplay();
}
