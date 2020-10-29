#ifndef MAINWINDOWCONTROLLER_H
#define MAINWINDOWCONTROLLER_H

#include <memory>
#include <QThread>

class CommunicationsManager;
class SystemDataSource;

class MainWindowController : public QObject
{
    Q_OBJECT

public:
    MainWindowController();
    ~MainWindowController();
    bool isStartServerButtonEnabled() const;
    bool isStopServerButtonEnabled() const;
    bool isStartTransmissionButtonEnabled() const;
    bool isStopTransmissionButtonEnabled() const;
    bool isPortLineEditEnabled() const;
    bool isPeriodicityLineEditEnabled() const;
    bool isOutboundDataTextEditEnabled() const;
    bool areEndianRadioButtonsEnabled() const;
    bool verifyTransmissionInterval(std::string interval) const;
    bool verifyPort(std::string port) const;
    void startServer(unsigned port, bool processBigEndian = false);
    void stopServer();
    void transmitOutboundData(std::string msg);
    void outboundTransmissionStopped();

public slots:
    void receivedStatusMessage(std::string msg);
    void showServerErrorMessage(std::string msg);
    void receivedDataFromClient(std::vector<unsigned> data);
    void receivedStatusChanged();

signals:
    void updateUI();
    void sendInboundDataToUI(std::string data);
    void updateInboundData(std::string msg);
    void sendStatusBarMessage(std::string msg);

    void requestStartServer(unsigned port, bool processBigEndian);
    void requestStopServer();
    void transmitData(std::vector<unsigned> data);

private:
    QThread serverThread;
    std::unique_ptr<CommunicationsManager> commsManager;
    std::unique_ptr<SystemDataSource> sds;
    bool transmittingDataToClient = false;
    bool outboundDataError = false;
    std::vector<unsigned> outboundData = {};

    void showUserInputErrorMessage(std::string msg) const;
    bool verifyOutboundData(std::string& msg);
};

#endif // MAINWINDOWCONTROLLER_H
