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
    void startServer(unsigned port, bool bigEndian);
    void stopServer();
    void sendDataToClient(std::vector<unsigned> data);

private slots:
    void incomingSocketConnection();
    void readIncomingData();
    void disconnectedFromSocket();

signals:
    void receivedDataFromClient(std::vector<unsigned> data);
    void sendStatusMessage(std::string msg);
    void sendErrorMessage(std::string msg);
    void statusChanged();

private:
    std::unique_ptr<QTcpServer> server;
    std::unique_ptr<QTcpSocket> socket;
    int socketPort = 0;
    bool serverListening = false;
    bool clientConnected = false;
    bool processBigEndianData = false;
    QReadWriteLock lock;

    QByteArray serializeOutboundData(std::vector<unsigned>& outData);
    std::vector<unsigned> deserializeInboundData(QByteArray inData);
};

#endif // COMMUNICATIONSMANAGER_H
