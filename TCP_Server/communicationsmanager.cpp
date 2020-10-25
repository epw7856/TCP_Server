#include "communicationsmanager.h"
#include "commutilities.h"
#include <QTcpServer>
#include <QTcpSocket>

CommunicationsManager::CommunicationsManager()
{

}

CommunicationsManager::~CommunicationsManager() = default;

bool CommunicationsManager::isServerListening()
{
    QReadLocker locker(&lock);
    return serverListening;
}

bool CommunicationsManager::isClientConnected()
{
    QReadLocker locker(&lock);
    return clientConnected;
}

void CommunicationsManager::startServer(unsigned port, bool bigEndian)
{
    processBigEndianData = bigEndian;
    socketPort = port;

    if(server == nullptr)
    {
        server = std::make_unique<QTcpServer>();
        connect(server.get(), &QTcpServer::newConnection, this, &CommunicationsManager::incomingSocketConnection);
    }

    QString msg = "";
    if(server->listen(QHostAddress::LocalHost, socketPort))
    {
        serverListening = true;
        msg = "Server is listening on port " + QString::number(socketPort) + "...";
        emit sendStatusMessage(msg.toStdString());
    }
    else
    {
        msg = "Unable to start server on port " + QString::number(socketPort) + "!";
        emit sendErrorMessage(msg.toStdString());
    }

    emit statusChanged();
}

void CommunicationsManager::stopServer()
{
    if(clientConnected)
    {
        socket->close();
    }

    if(serverListening)
    {
        server->close();
        serverListening = false;
    }

    QString msg = "Server stopped.";
    emit sendStatusMessage(msg.toStdString());
    emit statusChanged();
}

void CommunicationsManager::sendDataToClient(std::vector<unsigned> data)
{
    if(clientConnected && (data.size() > 0))
    {
        socket->write(serializeOutboundData(data));
        socket->flush();
    }
}

void CommunicationsManager::incomingSocketConnection()
{
    if(socket != nullptr)
    {
        socket->reset();
    }

    socket = std::make_unique<QTcpSocket>(server->nextPendingConnection());

    connect(socket.get(), &QTcpSocket::readyRead, this, &CommunicationsManager::readIncomingData);
    connect(socket.get(), &QTcpSocket::disconnected, this, &CommunicationsManager::disconnectedFromSocket);

    QString msg = "";
    if(socket->state() == QAbstractSocket::ConnectedState)
    {
        clientConnected = true;
        msg = "Connected to client on port " + QString::number(socket->peerPort()) + ".";
        emit sendStatusMessage(msg.toStdString());
    }
    else
    {
        msg = "Error encountered when connecting to client!";
        emit sendErrorMessage(msg.toStdString());
    }

    server->close();
    serverListening = false;

    emit statusChanged();
}

void CommunicationsManager::readIncomingData()
{
    emit receivedDataFromClient(deserializeInboundData(socket->readAll()));
}

void CommunicationsManager::disconnectedFromSocket()
{
    socket->reset();
    clientConnected = false;
    startServer(socketPort, processBigEndianData);

    emit statusChanged();
}

QByteArray CommunicationsManager::serializeOutboundData(std::vector<unsigned>& outData)
{
    QByteArray bytes;
    return (processBigEndianData) ? toByteArray(outData, QDataStream::BigEndian) :
                                    toByteArray(outData);
}

std::vector<unsigned> CommunicationsManager::deserializeInboundData(QByteArray inData)
{
    QDataStream ds(&inData, QIODevice::ReadOnly);
    processBigEndianData ? ds.setByteOrder(QDataStream::BigEndian) :
                           ds.setByteOrder(QDataStream::LittleEndian);

    ds.skipRawData(0);
    std::vector<unsigned> data;
    unsigned temp;
    unsigned size = inData.size() / sizeof(unsigned);

    for(unsigned i = 0; i < size; ++i)
    {
        ds >> temp;
        data.emplace_back(temp);
    }

    return data;
}
