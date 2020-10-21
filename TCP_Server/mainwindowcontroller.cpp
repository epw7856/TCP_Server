#include "communicationsmanager.h"
#include "mainwindowcontroller.h"
#include <QMessageBox>
#include <QValidator>

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
    connect(commsManager.get(), &CommunicationsManager::sendErrorMessage, this, &MainWindowController::showServerErrorMessage);
    connect(commsManager.get(), &CommunicationsManager::statusChanged, this, &MainWindowController::receivedStatusChanged);
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

bool MainWindowController::isStartTransmissionButtonEnabled() const
{
    return (commsManager->isClientConnected() && !transmittingDataToClient);
}

bool MainWindowController::isStopTransmissionButtonEnabled() const
{
    return (commsManager->isClientConnected() && transmittingDataToClient);
}

bool MainWindowController::isPortLineEditEnabled() const
{
    return isStartServerButtonEnabled();
}

bool MainWindowController::isPeriodicityLineEditEnabled() const
{
    return !transmittingDataToClient;
}

bool MainWindowController::isOutboundDataTextEditEnabled() const
{
    return !transmittingDataToClient;
}

bool MainWindowController::areEndianRadioButtonsEnabled() const
{
    return isStartServerButtonEnabled();
}

bool MainWindowController::verifyTransmissionInterval(std::string interval) const
{
    QDoubleValidator val(0.0001, 100000000.0000, 4);

    QString strInterval = QString::fromStdString(interval);
    int pos = 0;
    if(val.validate(strInterval, pos) != QValidator::Invalid)
    {
        return true;
    }
    else
    {
        showUserInputErrorMessage("Please enter a valid transmission interval.");
        return false;
    }
}

bool MainWindowController::verifyPort(std::string port) const
{
    QIntValidator val(1, 10000000);

    QString strPort = QString::fromStdString(port);
    int pos = 0;
    if(val.validate(strPort, pos) != QValidator::Invalid)
    {
        return true;
    }
    else
    {
        showUserInputErrorMessage("Please enter a valid port number.");
        return false;
    }
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
        transmittingDataToClient = true;
    }
}

void MainWindowController::showUserInputErrorMessage(std::string msg) const
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Input Error");
    msgBox.setText(QString::fromStdString(msg));
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.exec();
}

void MainWindowController::outboundTransmissionStopped()
{
    transmittingDataToClient = false;
}

void MainWindowController::receivedStatusMessage(std::string msg)
{
    emit sendStatusBarMessage(msg);
}

void MainWindowController::showServerErrorMessage(std::string msg)
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Server Error");
    msgBox.setText(QString::fromStdString(msg));
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.exec();
}

void MainWindowController::receivedDataFromClient(std::vector<unsigned> data)
{

}

void MainWindowController::receivedStatusChanged()
{
    emit updateUI();
}
