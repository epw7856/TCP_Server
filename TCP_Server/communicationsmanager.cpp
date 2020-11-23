#include "communicationsmanager.h"
#include "commutilities.h"
#include <QTcpSocket>

CommunicationsManager::CommunicationsManager() {}

CommunicationsManager::~CommunicationsManager() = default;

bool CommunicationsManager::isServerListening()
{
    QReadLocker locker(&lock);
    return isListening();
}

bool CommunicationsManager::isClientConnected()
{
    QReadLocker locker(&lock);
    return ((socket != nullptr) && (socket->state() == QAbstractSocket::ConnectedState));
}

void CommunicationsManager::startServer(unsigned port, bool bigEndian)
{
    QWriteLocker locker(&lock);

    abortSocketConnection();

    processBigEndianData = bigEndian;
    socketPort = port;

    QString msg = "";
    if(listen(QHostAddress::LocalHost, socketPort))
    {
        msg = "Server is listening on Port " + QString::number(socketPort) + "...";
        emit sendStatusMessage(msg.toStdString());
    }
    else
    {
        msg = "Unable to start server on Port " + QString::number(socketPort) + "!";
        emit sendErrorMessage("Server Error", msg.toStdString());
    }

    emit statusChanged();
}

void CommunicationsManager::stopServer()
{
    if(isClientConnected())
    {
        abortSocketConnection();
    }

    if(isServerListening())
    {
        lock.lockForWrite();
        close();
        lock.unlock();
    }

    QString msg = "Server stopped.";
    emit sendStatusMessage(msg.toStdString());
    emit statusChanged();
}

void CommunicationsManager::sendDataToClient(std::vector<unsigned> data)
{
    if(isClientConnected() && (data.size() > 0))
    {
        lock.lockForWrite();
        socket->write(serializeOutboundData(data));
        socket->flush();
        lock.unlock();
    }
}

void CommunicationsManager::readIncomingData()
{
    emit receivedDataFromClient(deserializeInboundData(socket->readAll()));
}

void CommunicationsManager::disconnectedFromSocket()
{
    startServer(socketPort, processBigEndianData);
    emit statusChanged();
}

void CommunicationsManager::incomingConnection(qintptr socketDescriptor)
{
    QWriteLocker locker(&lock);

    abortSocketConnection();

    socket = std::make_unique<QTcpSocket>();
    socket->setSocketDescriptor(socketDescriptor);

    connect(socket.get(), &QTcpSocket::readyRead, this, &CommunicationsManager::readIncomingData);
    connect(socket.get(), &QTcpSocket::disconnected, this, &CommunicationsManager::disconnectedFromSocket);

    QString msg = "";

    if(socket->state() == QAbstractSocket::ConnectedState)
    {
        msg = "Connected to client on Port " + QString::number(socket->localPort()) + ".";
        emit sendStatusMessage(msg.toStdString());
    }
    else
    {
        msg = "Error encountered when connecting to client!";
        emit sendErrorMessage("Server Error", msg.toStdString());
    }

    close();

    emit statusChanged();
}

void CommunicationsManager::abortSocketConnection()
{
    if(socket != nullptr)
    {
        socket->abort();
    }
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
