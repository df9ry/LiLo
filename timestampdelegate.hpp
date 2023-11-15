#ifndef TIMESTAMPDELEGATE_HPP
#define TIMESTAMPDELEGATE_HPP

#include <QStyledItemDelegate>

class TimestampDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit TimestampDelegate(QObject *parent = nullptr);

    virtual QString displayText(const QVariant & value, const QLocale & locale) const;
};

#endif // TIMESTAMPDELEGATE_HPP
