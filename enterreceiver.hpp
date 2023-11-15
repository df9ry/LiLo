#ifndef ENTERRECEIVER_HPP
#define ENTERRECEIVER_HPP

#include <QObject>

class EnterReceiver : public QObject
{
    Q_OBJECT

public:
    explicit EnterReceiver(QObject *parent = nullptr): QObject{parent} {};

signals:
    void onEnter();
    void onEscape();

protected:
    bool eventFilter(QObject *obj, QEvent *event);

};

#endif // ENTERRECEIVER_HPP
