#ifndef MAINWINDOWCONTROLLER_H
#define MAINWINDOWCONTROLLER_H

#include <QThread>

class MainWindowController
{
public:
    MainWindowController();
    void isStartServerButtonEnabled() const;
    void isStopServerButtonEnabled() const;
    void isPortLineEditEnabled() const;
    void isPeriodicityLineEditEnabled() const;
    void startServer(unsigned port);
    void stopServer();

signals:
    void updateUI();
    void sendStatusBarMessage(std::string msg);

private:
    QThread serverThread;

};

#endif // MAINWINDOWCONTROLLER_H
