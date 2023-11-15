#include "timestampdelegate.hpp"

#include <QDateTime>

TimestampDelegate::TimestampDelegate(QObject *parent)
    : QStyledItemDelegate{parent}
{
}

QString TimestampDelegate::displayText(const QVariant & value, const QLocale & locale) const
{
    if (value == 0)
        return QString();

    uint64_t secs{value.toULongLong()};
    QDateTime dt{QDateTime::fromSecsSinceEpoch(secs)};
    return dt.toString("dd.MM.yyyy hh:mm");
}
