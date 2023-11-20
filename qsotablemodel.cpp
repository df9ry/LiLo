#include "qsotablemodel.hpp"

#include <QSqlError>
#include <QSqlQuery>
#include <QSqlResult>

#include <cassert>

using namespace std;

typedef QVariant (*Getter_t)(const QsoTableModelItem &item);


static const char selectQuery[] {
    "SELECT id, station, timestamp, qrg, his, mine, mode, locator, name, qth, remarks"
    " FROM qsos ORDER BY timestamp DESC, id DESC"
};

QsoTableModel::QsoTableModel(QSqlDatabase &db, QObject *parent)
    : QSqlQueryModel(parent), database{db}
{
    for (int i = 0; i < size(columns); ++i) {
        const ColumnInfo_t &column{columns[i]};
        setHeaderData(i, Qt::Horizontal, column.label);
    }
}

QsoTableModel::~QsoTableModel() {
    database.close();
}


int QsoTableModel::columnCount(const QModelIndex & /*parent*/) const
{
    return size(columns);
}

QVariant QsoTableModel::data(const QModelIndex &index, int role) const
{
    return QSqlQueryModel::data(index, role);
}

QVariant QsoTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (section < size(columns)) {
        if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
            return columns[section].label;
    }
    return QVariant();
}

bool QsoTableModel::start()
{
    if (!database.open()) {
        setLastError(database.lastError());
        return false;
    }
    const QStringList tables = database.tables();
    if (!tables.contains("stat")) {
        {
            qDebug() << "Creating stat table";
            QSqlQuery query("CREATE TABLE stat ("
                "id INTEGER PRIMARY KEY,"
                "next_id INTEGER NOT NULL"
                ");",
                database
            );
            if (query.exec())
            {
                setLastError(query.lastError());
                next_id = -1;
                return false;
            }
        }
        {
            qDebug() << "Filling in stat table";
            QSqlQuery query("INSERT INTO stat (id, next_id) VALUES(0, 0);",
                            database);
            if (query.exec())
            {
                setLastError(query.lastError());
                next_id = -1;
                return false;
            }
        }
        next_id = 0;
    } else {
        QSqlQuery query("SELECT next_id FROM stat;", database);
        if (!query.exec()) {
            setLastError(query.lastError());
            next_id = -1;
            return false;
        }
        if (!query.first()) {
            setLastError(query.lastError());
            next_id = -1;
            return false;
        }
        next_id = query.value(0).toInt();
    }

    if (!tables.contains("qsos")) {
        qDebug() << "Creating qsos table";
        QSqlQuery query(
            "CREATE TABLE qsos ("
            "id INTEGER PRIMARY KEY,"
            "station TEXT NOT NULL,"
            "timestamp BIGINT UNSIGNED,"
            "qrg INTEGER,"
            "his TEXT,"
            "mine TEXT,"
            "mode TEXT,"
            "locator TEXT,"
            "name TEXT,"
            "qth TEXT,"
            "remarks TEXT"
            ");",
            database
        );
        if (query.exec())
        {
            setLastError(query.lastError());
            next_id = -1;
            return false;
        }
    }

    setQuery(selectQuery, database);
    if (lastError().isValid()) {
        qDebug() << lastError();
        return false;
    }

    return true;
}

QsoTableModelItem &QsoTableModel::newQSO()
{
    qso.id = next_id;
    return qso;
}

bool QsoTableModel::saveOSO(QsoTableModelItem &qso)
{
    if (qso.id < 0) {
        setLastError(QSqlError(tr("QSO not opened"),
                               database.databaseName(),
                               QSqlError::ErrorType::TransactionError,
                               "ID < 0"));
        return false;
    }

    if (!database.transaction()) {
        setLastError(QSqlError(tr("QSO not opened"),
                               database.databaseName(),
                               QSqlError::ErrorType::TransactionError,
                               "Unable to start transaction"));
        return false;
    }

    beginResetModel();

    QSqlQuery query1(database);
    query1.prepare("INSERT INTO qsos "
                    "( id, station, timestamp, qrg, his, mine, mode, locator, name, qth, remarks) "
                    "VALUES "
                    "(:id,:station,:timestamp,:qrg,:his,:mine,:mode,:locator,:name,:qth,:remarks);");
    query1.bindValue(":id",        QVariant::fromValue(qso.id       ));
    query1.bindValue(":station",   QVariant::fromValue(qso.station  ));
    query1.bindValue(":timestamp", QVariant::fromValue(qso.timestamp));
    query1.bindValue(":qrg",       QVariant::fromValue(qso.qrg      ));
    query1.bindValue(":his",       QVariant::fromValue(qso.his      ));
    query1.bindValue(":mine",      QVariant::fromValue(qso.mine     ));
    query1.bindValue(":mode",      QVariant::fromValue(qso.mode     ));
    query1.bindValue(":locator",   QVariant::fromValue(qso.locator  ));
    query1.bindValue(":name",      QVariant::fromValue(qso.name     ));
    query1.bindValue(":qth",       QVariant::fromValue(qso.qth      ));
    query1.bindValue(":remarks",   QVariant::fromValue(qso.remarks  ));
    query1.finish();

    //QVariantList list = query1.boundValues();
    //for (int i = 0; i < list.size(); ++i)
    //    qDebug() << i << list.at(i);

    if (!query1.exec()) {
        setLastError(database.lastError());
        database.rollback();
        endResetModel();
        return false;
    }

    QSqlQuery query2(database);
    query2.prepare("UPDATE stat SET next_id = :next_id WHERE id = 0");
    query2.bindValue(":next_id", next_id + 1);
    query2.finish();
    if (!query2.exec()) {
        setLastError(database.lastError());
        database.rollback();
        endResetModel();
        return false;
    }

    if (!database.commit()) {
        setLastError(database.lastError());
        database.rollback();
        endResetModel();
        return false;
    }
    next_id += 1;

    setQuery(selectQuery, database);
    if (lastError().isValid()) {
        setLastError(database.lastError());
        qDebug() << lastError();
        endResetModel();
        return false;
    }

    endResetModel();

    return true;
}

void QsoTableModel::cancelQSO(QsoTableModelItem &qso)
{
    qso.id = -1;
}
