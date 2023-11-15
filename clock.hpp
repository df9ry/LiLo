#ifndef CLOCK_HPP
#define CLOCK_HPP

#include <QDialog>

#include "iclocktick.hpp"

namespace Ui {
class Clock;
}

class MainWindow;

class Clock : public QDialog, IClockTick
{
    Q_OBJECT

public:
    explicit Clock(MainWindow *parent);
    ~Clock();

    void tick(const QDateTime &dt) override;

private:
    Ui::Clock  *ui;
    MainWindow *parent;
    int         day{-1};
};

#endif // CLOCK_HPP
