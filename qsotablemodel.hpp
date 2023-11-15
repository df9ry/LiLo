#ifndef QSOTABLEMODEL_HPP
#define QSOTABLEMODEL_HPP

#include <QSqlQueryModel>

#include "qsotablemodelitem.hpp"

class QsoTableModel : public QSqlQueryModel
{
    Q_OBJECT
public:

    typedef enum {
        INT, LABEL, TIMESTAMP, TEXT
    } Type_t;

    typedef struct {
        const QString  label;
        const bool     visible;
        const Type_t   type;
    } ColumnInfo_t;

    const ColumnInfo_t columns[11] {
        { QObject::tr("ID"),        false, INT       },
        { QObject::tr("Station"),   true,  LABEL     },
        { QObject::tr("Timestamp"), true,  TIMESTAMP },
        { QObject::tr("QRG [kHz]"), true,  LABEL     },
        { QObject::tr("His"),       true,  LABEL     },
        { QObject::tr("Mine"),      true,  LABEL     },
        { QObject::tr("Mode"),      true,  LABEL     },
        { QObject::tr("Locator"),   true,  LABEL     },
        { QObject::tr("Name"),      true,  LABEL     },
        { QObject::tr("QTH"),       true,  LABEL     },
        { QObject::tr("Remarks"),   true,  TEXT      },
    };

    explicit QsoTableModel(QSqlDatabase &db, QObject *parent = nullptr);
    ~QsoTableModel();

    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    bool start();
    QsoTableModelItem &newQSO();
    bool saveOSO(QsoTableModelItem &qso);
    void cancelQSO(QsoTableModelItem &qso);

private:
    QSqlDatabase &database;
    int next_id{-1};
    QsoTableModelItem qso{};
};

#endif // QSOTABLEMODEL_HPP
