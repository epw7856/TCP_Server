#include "communicationsmanager.h"
#include "mainwindowcontroller.h"
#include <QMessageBox>
#include <QValidator>
#include "systemdatasource.h"

Q_DECLARE_METATYPE(std::string);

MainWindowController::MainWindowController()
:
    commsManager(std::make_unique<CommunicationsManager>()),
    sds(std::make_unique<SystemDataSource>())
{
    qRegisterMetaType<std::string>();

    commsManager->moveToThread(&serverThread);

    connect(this, &MainWindowController::requestStartServer, commsManager.get(), &CommunicationsManager::startServer);
    connect(this, &MainWindowController::requestStopServer, commsManager.get(), &CommunicationsManager::stopServer);
    connect(commsManager.get(), &CommunicationsManager::sendStatusMessage, this, &MainWindowController::receivedStatusMessage);
    connect(commsManager.get(), &CommunicationsManager::sendErrorMessage, this, &MainWindowController::showErrorMessage);
    connect(commsManager.get(), &CommunicationsManager::statusChanged, this, &MainWindowController::receivedStatusChanged);
    connect(this, &MainWindowController::transmitData, commsManager.get(), &CommunicationsManager::sendDataToClient);
    connect(commsManager.get(), &CommunicationsManager::receivedDataFromClient, this, &MainWindowController::receivedDataFromClient);

    connect(sds.get(), &SystemDataSource::sendErrorMessage, this, &MainWindowController::showErrorMessage);

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
    return (commsManager->isClientConnected() && sds->areDataTypesValid() && !transmittingDataToClient);
}

bool MainWindowController::isStopTransmissionButtonEnabled() const
{
    return (commsManager->isClientConnected() && sds->areDataTypesValid() && transmittingDataToClient);
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

bool MainWindowController::verifyTransmissionInterval(std::string interval)
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
        showErrorMessage("Input Error", "Please enter a valid transmission interval.");
        return false;
    }
}

bool MainWindowController::verifyPort(std::string port)
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
        showErrorMessage("Input Error", "Please enter a valid port number.");
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

void MainWindowController::transmitOutboundData(std::string outData)
{
    if(commsManager->isClientConnected())
    {
        transmittingDataToClient = true;

        if(verifyOutboundData(outData))
        {
            emit transmitData(outboundData);
            outboundDataError = false;
        }
        else if(!outboundDataError)
        {
            showErrorMessage("Transmission Error", "Invalid outbound data detected!");
            outboundDataError = true;
        }
    }
}

bool MainWindowController::verifyOutboundData(std::string& outData)
{
    outboundData.clear();
    return sds->convertOutboundData(outboundData, outData);
}

void MainWindowController::outboundTransmissionStopped()
{
    transmittingDataToClient = false;
    outboundDataError = false;
}

void MainWindowController::receivedStatusMessage(std::string msg)
{
    emit sendStatusBarMessage(msg);
}

void MainWindowController::showErrorMessage(std::string title, std::string msg)
{
    QMessageBox msgBox;
    msgBox.setWindowTitle(QString::fromStdString(title));
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
