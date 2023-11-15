#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QColor>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
class QsoTableModel;
class EnterReceiver;
class QTimer;
class Clock;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool start();

private:
    Ui::MainWindow *ui;
    QsoTableModel  *qsoTableModel;
    EnterReceiver  *enterReceiver;
    QTimer         *tickTimer;
    QColor          fieldBackgroundColor{};
    Clock          *clock{};

    void closeEvent(QCloseEvent *event);

    bool maybeSave();
    void writeSettings();
    void saveQSO();
    void cancelQSO();
    void resetInputFields();
    void tick();
    bool isValidMaidenhead(const QString &locator) const;
    bool isValidCallsign(const QString &callsign) const;
    bool isValidStation(const QString &station) const;
    bool isValidRapport(const QString &rapport) const;

    friend class Clock;
};
#endif // MAINWINDOW_HPP
