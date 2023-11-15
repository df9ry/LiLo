#include "singlelinelabel.hpp"

#include <QLabel>

SingleLineLabel::SingleLineLabel(QWidget *parent):
    QStyledItemDelegate(parent)
{
}

QString SingleLineLabel::displayText(const QVariant & value, const QLocale & locale) const
{
    if (value == 0)
        return QString();

    return value.toString().simplified();
}
