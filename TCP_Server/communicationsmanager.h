#ifndef COMMUNICATIONSMANAGER_H
#define COMMUNICATIONSMANAGER_H

#include <memory>
#include <QReadWriteLock>
#include <QTcpServer>

class QTcpSocket;

class CommunicationsManager : public QTcpServer
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
    void readIncomingData();
    void disconnectedFromSocket();

protected:
    void incomingConnection(qintptr socketDescriptor) override;

signals:
    void receivedDataFromClient(std::vector<unsigned> data);
    void sendStatusMessage(std::string msg);
    void sendErrorMessage(std::string title, std::string msg);
    void statusChanged();

private:
    std::unique_ptr<QTcpSocket> socket;
    int socketPort = 0;
    bool clientConnected = false;
    bool processBigEndianData = false;
    QReadWriteLock lock;

    QByteArray serializeOutboundData(std::vector<unsigned>& outData);
    std::vector<unsigned> deserializeInboundData(QByteArray inData);
};

#endif // COMMUNICATIONSMANAGER_H
