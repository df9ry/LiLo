#include "clock.hpp"
#include "ui_clock.h"

#include "mainwindow.hpp"

Clock::Clock(MainWindow *_parent) :
    QDialog(_parent),
    IClockTick(),
    ui(new Ui::Clock),
    parent{_parent}
{
    ui->setupUi(this);
    QPixmap bkgnd(":/res/world.png");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Window, bkgnd);
    this->setPalette(palette);
    this->setWindowFlag(Qt::Dialog, false);
    this->setWindowFlag(Qt::Window, true);
}

Clock::~Clock()
{
    delete ui;
    parent->clock = nullptr;
}

void Clock::tick(const QDateTime &dt)
{
    int h = dt.time().hour();
    int m = dt.time().minute();
    int s = dt.time().second();
    ui->digit1->display(h / 10);
    ui->digit2->display(h % 10);
    ui->digit3->display(m / 10);
    ui->digit4->display(m % 10);
    ui->digit5->display(s / 10);
    ui->digit6->display(s % 10);
    int d = dt.date().day();
    if (d != day) {
        ui->date->setText(
            QLocale::system().toString(dt.date(), "dddd d. MMMM yyyy")
        );
        day = d;
    }
}
