#include "mainwindow.hpp"
#include "./ui_mainwindow.h"
#include "about.hpp"
#include "clock.hpp"
#include "databasesupport.hpp"
#include "qsotablemodel.hpp"
#include "enterreceiver.hpp"
#include "singlelinelabel.hpp"
#include "timestampdelegate.hpp"

#include <QCloseEvent>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QTimer>
#include <QRegularExpression>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->action_Qt, &QAction::triggered, this, []() {
        QApplication::aboutQt();
    });
    connect(ui->action_LiLo, &QAction::triggered, this, [this]() {
            About dlg(this);
            dlg.setModal(true);
            dlg.exec();
    });
    connect(ui->action_Clock, &QAction::triggered, this, [this]() {
        if (!clock) {
            clock = new Clock(this);
            clock->setModal(false);
        }
        clock->show();
    });

    ::mainDB.setDatabaseName("test.db");

    qsoTableModel = new QsoTableModel(::mainDB, this);
    ui->qsoTableView->setModel(qsoTableModel);
    ui->qsoTableView->resizeColumnsToContents();
    ui->qsoTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->qsoTableView->setSelectionMode(QAbstractItemView::SingleSelection);

    enterReceiver = new EnterReceiver(this);
    ui->entryframe->installEventFilter(enterReceiver);
    ui->remarksEdit->installEventFilter(enterReceiver);
    connect(enterReceiver, &EnterReceiver::onEnter, this, [this]() {
        saveQSO();
    });
    connect(enterReceiver, &EnterReceiver::onEscape, this, [this]() {
        cancelQSO();
    });
    connect(ui->stationField, &QLineEdit::textEdited, this, [this](const QString& text)
    {
        QString t1{text.toUpper()};
        QString t2{""};
        for (int i = 0; i < t1.length(); ++i) {
            auto ch{t1.at(i)};
            if (((ch >= 'A') && (ch <= 'Z')) || ((ch >= '0') && (ch <= '9')) || (ch == '/')) {
                t2.append(ch);
            }
        } // end for //
        ui->stationField->setText(t2);
        QColor backgroundColor{isValidStation(t2) ? Qt::green : fieldBackgroundColor};
        QString style{"background-color:" + backgroundColor.name() + ";"};
        ui->stationField->setStyleSheet(style);
    });
    connect(ui->modeField, &QLineEdit::textEdited, this, [this](const QString& text)
    {
        ui->modeField->setText(text.toUpper());
    });
    connect(ui->hisField, &QLineEdit::textEdited, this, [this](const QString& text)
    {
        QColor backgroundColor{isValidRapport(text) ? Qt::green : fieldBackgroundColor};
        QString style{"background-color:" + backgroundColor.name() + ";"};
        ui->hisField->setStyleSheet(style);
    });
    connect(ui->mineField, &QLineEdit::textEdited, this, [this](const QString& text)
    {
        QColor backgroundColor{isValidRapport(text) ? Qt::green : fieldBackgroundColor};
        QString style{"background-color:" + backgroundColor.name() + ";"};
        ui->mineField->setStyleSheet(style);
    });
    connect(ui->locatorField, &QLineEdit::textEdited, this, [this](const QString& text)
    {
        QString newText;
        if (text.length() <= 4) {
            newText = text.toUpper();
        } else {
            QString t1 = text.left(4).toUpper();
            QString t2 = text.mid(4).toLower();
            newText = t1 + t2;
        }
        ui->locatorField->setText(newText);
        QColor backgroundColor{isValidMaidenhead(newText) ? Qt::green : fieldBackgroundColor};
        QString style{"background-color:" + backgroundColor.name() + ";"};
        ui->locatorField->setStyleSheet(style);
    });
    connect(ui->timestampLock, &QCheckBox::toggled, this, [this](bool checked) {
        tick();
        ui->timestampField->setReadOnly(!checked);
        QColor backgroundColor{checked ? Qt::yellow : fieldBackgroundColor};
        QString style{"background-color:" + backgroundColor.name() + ";"};
        ui->timestampField->setStyleSheet(style);
        ui->timestampLock->setStyleSheet(style);
    });
    fieldBackgroundColor = ui->entryframe->palette().base().color();
    tickTimer = new QTimer(this);
    tickTimer->setInterval(1000);
    connect(tickTimer, &QTimer::timeout, this, [this]() {
        tick();
    });
}

MainWindow::~MainWindow()
{
    delete tickTimer;
    delete qsoTableModel;
    delete ui;
    delete enterReceiver;
}

bool MainWindow::start()
{
    if (!qsoTableModel->start()) {
        QMessageBox::critical(this, tr("Error"), qsoTableModel->lastError().text());
        return false;
    }
    for (int col = 0; col < qsoTableModel->columnCount(); ++col) {
        const QsoTableModel::ColumnInfo_t &info{qsoTableModel->columns[col]};
        switch (info.type) {
        case QsoTableModel::Type_t::TEXT:
            ui->qsoTableView->setItemDelegateForColumn(col, new SingleLineLabel(this));
            break;
        case QsoTableModel::Type_t::TIMESTAMP:
            ui->qsoTableView->setItemDelegateForColumn(col, new TimestampDelegate(this));
            break;
        default:
            break;
        }; // end switch //
        ui->qsoTableView->setColumnHidden(col, !info.visible);
    } // end for //

    ui->qsoTableView->resizeRowsToContents();
    ui->qsoTableView->resizeColumnsToContents();
    ui->qsoTableView->horizontalHeader()->setStretchLastSection(true);
    tickTimer->start();
    tick();
    return true;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSave()) {
        writeSettings();
        event->accept();
    } else {
        event->ignore();
    }
}

bool MainWindow::maybeSave()
{
    return true;
}

void MainWindow::writeSettings()
{
    qDebug() << "Write settings";
}

void MainWindow::saveQSO()
{
    qDebug() << "Save QSO";
    QString   station   = ui->stationField->text();
    if (station.isEmpty()) {
        cancelQSO();
        return;
    }
    QsoTableModelItem &item = qsoTableModel->newQSO();
    item.station   = station;
    item.timestamp = ui->timestampField->dateTime().toSecsSinceEpoch();
    item.his       = ui->hisField->text();
    item.mine      = ui->mineField->text();
    item.qrg       = ui->qrgField->value();
    item.mode      = ui->modeField->text();
    item.locator   = ui->locatorField->text();
    item.name      = ui->nameField->text();
    item.qth       = ui->qthField->text();
    item.remarks   = ui->remarksEdit->toPlainText();
    bool ok = qsoTableModel->saveOSO(item);
    if (!ok) {
        QSqlError err = qsoTableModel->lastError();
        QMessageBox::critical(this, tr("Error"),
                                    tr("Unable to save QSO: %1").arg(err.text()));
    }
    ui->qsoTableView->resizeRowsToContents();
    ui->qsoTableView->resizeColumnsToContents();
    ui->qsoTableView->horizontalHeader()->setStretchLastSection(true);
    resetInputFields();
}

void MainWindow::cancelQSO()
{
    qDebug() << "Cancel QSO";
    resetInputFields();
}

void MainWindow::resetInputFields()
{
    qDebug() << "ResetInputFields";
    ui->stationField->setText("");
    ui->hisField->setText("");
    ui->mineField->setText("");
    ui->timestampLock->setCheckState(Qt::CheckState::Unchecked);
    ui->locatorField->setText("");
    ui->nameField->setText("");
    ui->qthField->setText("");
    ui->remarksEdit->setText("");
}

bool MainWindow::isValidMaidenhead(const QString &locator) const
{
    static const QRegularExpression rex{"^[A-R][A-R][0-9][0-9]([a-x][a-x])?$"};
    return rex.match(locator).hasMatch();
}

bool MainWindow::isValidCallsign(const QString &callsign) const
{
    static const QRegularExpression rex{"^[0-9]*[A-Z]+[0-9]+[A-Z]+$"};
    return rex.match(callsign).hasMatch();
}

bool MainWindow::isValidStation(const QString &station) const
{
    return isValidCallsign(station);
}

bool MainWindow::isValidRapport(const QString &rapport) const
{
    static const QRegularExpression rex{"^[0-5][0-9].*$"};
    return rex.match(rapport).hasMatch();
}

void MainWindow::tick() {
    QDateTime now = QDateTime::currentDateTimeUtc();
    if (!ui->timestampLock->isChecked()) {
        ui->timestampField->setDateTime(now);
    }
    if (clock) {
        clock->tick(now);
    }
}
