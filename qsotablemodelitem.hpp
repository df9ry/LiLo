#ifndef QSOTABLEMODELITEM_HPP
#define QSOTABLEMODELITEM_HPP


#include <QString>

class QsoTableModel;

class QsoTableModelItem
{
public:

    bool isValid() const { return id >= 0; }

    QString   station{};
    uint64_t  timestamp{};
    uint32_t  qrg{};
    QString   his{};
    QString   mine{};
    QString   mode{};
    QString   locator{};
    QString   name{};
    QString   qth{};
    QString   remarks{};

private:
    QsoTableModelItem() {};
    int       id{-1};

    friend class QsoTableModel;
};

#endif // QSOTABLEMODELITEM_HPP
