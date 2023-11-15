#include "enterreceiver.hpp"

#include <QEvent>
#include <QKeyEvent>

bool EnterReceiver::eventFilter(QObject *obj, QEvent *event) {
    if(event->type() != QEvent::KeyPress)
        return QObject::eventFilter(obj, event);
    QKeyEvent *key = static_cast<QKeyEvent*>(event);
    if(key->key() == Qt::Key_Enter) {
        qDebug() << "Enter was pressed";
        emit onEnter();
        return true;
    }
    if(key->key() == Qt::Key_Escape) {
        qDebug() << "Escape was pressed";
        emit onEscape();
        return true;
    }
    return QObject::eventFilter(obj, event);
}
