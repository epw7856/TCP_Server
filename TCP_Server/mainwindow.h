#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>
#include <QMainWindow>

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
    void showStatusBarMessage(QString msg);
    void periodicUpdate();

private slots:
    void onActionExitTriggered();
    void onPushButtonStartServerClicked();
    void onPushButtonStopServerClicked();

private:
    Ui::MainWindow *ui;
    std::unique_ptr<MainWindowController> controller;
    std::unique_ptr<QLabel> statusBarLabel;

    void setupStatusBar();
};
#endif // MAINWINDOW_H
