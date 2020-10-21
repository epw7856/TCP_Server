#include "communicationsmanager.h"
#include "mainwindowcontroller.h"
#include <QMessageBox>

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
    connect(commsManager.get(), &CommunicationsManager::sendErrorMessage, this, &MainWindowController::receivedErrorMessage);
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
    return !(commsManager->isServerListening() || commsManager->isClientConnected());
}

bool MainWindowController::isStopServerButtonEnabled() const
{
    return (commsManager->isServerListening() || commsManager->isClientConnected());
}

bool MainWindowController::isPortLineEditEnabled() const
{
    return isStartServerButtonEnabled();
}

bool MainWindowController::isPeriodicityLineEditEnabled() const
{
    return isStartServerButtonEnabled();
}

void MainWindowController::startServer(unsigned port, bool processBigEndian)
{
    emit requestStartServer(port, processBigEndian);
}

void MainWindowController::stopServer()
{
    emit requestStopServer();
}

void MainWindowController::transmitOutboundData(std::string msg)
{
    if(commsManager->isClientConnected())
    {

    }
}

void MainWindowController::receivedStatusMessage(std::string msg)
{
    emit sendStatusBarMessage(msg);
}

void MainWindowController::receivedErrorMessage(std::string msg)
{
    QMessageBox msgBox;
    msgBox.setText(QString::fromStdString(msg));
    msgBox.setWindowTitle("Server Error");
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.exec();
}

void MainWindowController::receivedDataFromClient(std::vector<unsigned> data)
{

}
