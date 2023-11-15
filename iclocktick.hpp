#ifndef ICLOCKTICK_HPP
#define ICLOCKTICK_HPP

#include <QDateTime>

class IClockTick {
public:
    virtual void tick(const QDateTime &dt) = 0;
};

#endif // ICLOCKTICK_HPP
