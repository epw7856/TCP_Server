#include "mainwindowcontroller.h"

Q_DECLARE_METATYPE(std::string);

MainWindowController::MainWindowController()
{
    qRegisterMetaType<std::string>();
}

void MainWindowController::isStartServerButtonEnabled() const
{

}

void MainWindowController::isStopServerButtonEnabled() const
{

}

void MainWindowController::isPortLineEditEnabled() const
{

}

void MainWindowController::isPeriodicityLineEditEnabled() const
{

}

void MainWindowController::startServer(unsigned port)
{

}

void MainWindowController::stopServer()
{

}
