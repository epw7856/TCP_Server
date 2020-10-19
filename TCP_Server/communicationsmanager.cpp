#include "communicationsmanager.h"
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

void CommunicationsManager::startServer()
{

}

void CommunicationsManager::stopServer()
{

}

void CommunicationsManager::sendDataToClient(std::vector<unsigned> data)
{

}

void CommunicationsManager::incomingSocketConnection()
{

}

void CommunicationsManager::readIncomingData()
{

}

void CommunicationsManager::disconnectedFromSocket()
{

}
