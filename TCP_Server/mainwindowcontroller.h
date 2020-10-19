#ifndef MAINWINDOWCONTROLLER_H
#define MAINWINDOWCONTROLLER_H

#include <memory>
#include <QThread>

class CommunicationsManager;

class MainWindowController : public QObject
{
    Q_OBJECT

public:
    MainWindowController();
    ~MainWindowController();
    bool isStartServerButtonEnabled() const;
    bool isStopServerButtonEnabled() const;
    bool isPortLineEditEnabled() const;
    bool isPeriodicityLineEditEnabled() const;
    void startServer(unsigned port);
    void stopServer();
    void transmitOutboundData(std::string msg, bool isLittleEndian = true);

public slots:
    void receivedStatusMessage(std::string msg);
    void receivedDataFromClient(std::vector<unsigned> data);

signals:
    void updateUI();
    void updateInboundData(std::string msg);
    void sendStatusBarMessage(std::string msg);

    void requestStartServer();
    void requestStopServer();
    void transmitData(std::vector<unsigned> data);

private:
    QThread serverThread;
    std::unique_ptr<CommunicationsManager> commsManager;

};

#endif // MAINWINDOWCONTROLLER_H
