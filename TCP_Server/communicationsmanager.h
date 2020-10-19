#ifndef COMMUNICATIONSMANAGER_H
#define COMMUNICATIONSMANAGER_H

#include <memory>
#include <QReadWriteLock>
#include <QObject>

class QTcpServer;
class QTcpSocket;

class CommunicationsManager : public QObject
{
    Q_OBJECT

public:
    CommunicationsManager();
    ~CommunicationsManager();
    bool isServerListening();
    bool isClientConnected();

public slots:
    void startServer();
    void stopServer();
    void sendDataToClient(std::vector<unsigned> data);

private slots:
    void incomingSocketConnection();
    void readIncomingData();
    void disconnectedFromSocket();

signals:
    void receivedDataFromClient(std::vector<unsigned> data);
    void serverStopped();
    void sendStatusMessage(std::string msg);

private:
    std::unique_ptr<QTcpServer> server;
    std::unique_ptr<QTcpSocket> socket;
    bool serverListening = false;
    bool clientConnected = false;
    QReadWriteLock lock;
};

#endif // COMMUNICATIONSMANAGER_H
