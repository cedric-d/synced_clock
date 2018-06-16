#ifndef STANDARDCLOCK_H
#define STANDARDCLOCK_H

#include "clock.h"

#include <QMainWindow>
#include <QObject>

namespace Ui {
class StandardClock;
}

class StandardClock : public Clock
{
    Q_OBJECT

public:
    explicit StandardClock(QObject *parent = Q_NULLPTR);
    ~StandardClock();

protected:
    virtual void updateDisplay() Q_DECL_OVERRIDE;

private:
    QMainWindow *window;
    Ui::StandardClock *ui;
};

#endif // STANDARDCLOCK_H
