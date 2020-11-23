#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>
#include <QMainWindow>
#include <QTimer>

class MainWindowController;
class QLabel;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void showStatusBarMessage(std::string msg);
    void periodicUpdate();
    void updateInboundData(std::string data);

private slots:
    void onActionExitTriggered();
    void onPushButtonStartServerClicked();
    void onPushButtonStopServerClicked();
    void onPushButtonStartTransmissionClicked();
    void onPushButtonStopTransmissionClicked();
    void transmitOutboundData();
    void requestUpdatePort();
    void requestUpdateTransmissionInterval();

private:
    Ui::MainWindow *ui;
    std::unique_ptr<MainWindowController> controller;
    std::unique_ptr<QLabel> statusBarLabel;
    QTimer transmissionTimer;

    void setupStatusBar();
    void stopTransmissions();
};
#endif // MAINWINDOW_H
