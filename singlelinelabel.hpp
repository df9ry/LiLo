#ifndef SINGLELINELABEL_HPP
#define SINGLELINELABEL_HPP

#include <QStyledItemDelegate>

class SingleLineLabel : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit SingleLineLabel(QWidget *parent = nullptr);

    virtual QString displayText(const QVariant & value, const QLocale & locale) const;
};

#endif // SINGLELINELABEL_HPP
