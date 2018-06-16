#include "standardclock.h"
#include "ui_standardclock.h"

#include <QString>
#include <QtDebug>

StandardClock::StandardClock(QObject *parent) :
    Clock(parent),
    window(new QMainWindow),
    ui(new Ui::StandardClock)
{
    this->ui->setupUi(this->window);
    this->window->adjustSize();
    this->window->show();
}

StandardClock::~StandardClock()
{
    delete this->ui;
    delete this->window;
}

void StandardClock::updateDisplay()
{
    const QTime &now = this->getNow();
    const QTime &prev = this->getPrevious();

    this->ui->lblCurrent->setText(now.toString("HH:mm:ss.zzz"));
    this->ui->lblPrevious->setText(prev.toString("HH:mm:ss.zzz"));
    this->ui->lblDelta->setText(tr("%L1 ms").arg(prev.msecsTo(now)));
    this->ui->lblCounter->setText(QString::number(this->getCounter()));
    this->ui->lblRate->setText(tr("%L1 Hz").arg(this->getRate(), 0, 'f', 1));
}
