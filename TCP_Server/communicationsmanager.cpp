#include "communicationsmanager.h"
#include <QTcpServer>
#include <QTcpSocket>

CommunicationsManager::CommunicationsManager()
{

}

CommunicationsManager::~CommunicationsManager() = default;

bool CommunicationsManager::isServerListening() const
{
    return serverListening;
}

bool CommunicationsManager::isClientConnected() const
{
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
