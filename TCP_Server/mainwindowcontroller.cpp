#include "communicationsmanager.h"
#include "mainwindowcontroller.h"

Q_DECLARE_METATYPE(std::string);

MainWindowController::MainWindowController()
:
    commsManager(std::make_unique<CommunicationsManager>())
{
    qRegisterMetaType<std::string>();

    commsManager->moveToThread(&serverThread);

    connect(this, &MainWindowController::requestStartServer, commsManager.get(), &CommunicationsManager::startServer);
    connect(this, &MainWindowController::requestStopServer, commsManager.get(), &CommunicationsManager::stopServer);
    connect(commsManager.get(), &CommunicationsManager::sendStatusMessage, this, &MainWindowController::receivedStatusMessage);
    connect(this, &MainWindowController::transmitData, commsManager.get(), &CommunicationsManager::sendDataToClient);
    connect(commsManager.get(), &CommunicationsManager::receivedDataFromClient, this, &MainWindowController::receivedDataFromClient);

    serverThread.start();
}

MainWindowController::~MainWindowController()
{
    emit requestStopServer();
    serverThread.quit();
    serverThread.wait(2000);
}

bool MainWindowController::isStartServerButtonEnabled() const
{
    return true;
}

bool MainWindowController::isStopServerButtonEnabled() const
{
    return true;
}

bool MainWindowController::isPortLineEditEnabled() const
{
    return true;
}

bool MainWindowController::isPeriodicityLineEditEnabled() const
{
    return true;
}

void MainWindowController::startServer(unsigned port)
{

}

void MainWindowController::stopServer()
{

}

void MainWindowController::transmitOutboundData(std::string msg, bool isLittleEndian)
{

}

void MainWindowController::receivedStatusMessage(std::string msg)
{

}

void MainWindowController::receivedDataFromClient(std::vector<unsigned> data)
{

}
