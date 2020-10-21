#include "mainwindow.h"
#include "mainwindowcontroller.h"
#include <QLabel>
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
:
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    controller(std::make_unique<MainWindowController>())
{
    ui->setupUi(this);

    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::onActionExitTriggered);
    connect(ui->pushButtonStartServer, &QPushButton::clicked, this, &MainWindow::onPushButtonStartServerClicked);
    connect(ui->pushButtonStopServer, &QPushButton::clicked, this, &MainWindow::onPushButtonStopServerClicked);

    connect(controller.get(), &MainWindowController::updateUI, this, &MainWindow::periodicUpdate);
    connect(controller.get(), &MainWindowController::sendInboundDataToUI, this, &MainWindow::updateInboundData);
    connect(controller.get(), &MainWindowController::sendStatusBarMessage, this, &MainWindow::showStatusBarMessage);

    connect(&transmissionTimer, &QTimer::timeout, this, &MainWindow::transmitOutboundData);

    setupStatusBar();
    periodicUpdate();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupStatusBar()
{
    statusBarLabel = std::make_unique<QLabel>();
    statusBarLabel->setFont(QFont("Segoe UI", 10));

    ui->statusBar->setStyleSheet("QStatusBar{border-top: 1px outset grey;}");
    ui->statusBar->addPermanentWidget(statusBarLabel.get());
    ui->statusBar->setSizeGripEnabled(false);
    showStatusBarMessage("Ready");
}

void MainWindow::showStatusBarMessage(std::string msg)
{
    statusBarLabel->clear();
    statusBarLabel->setText("Status: " + QString::fromStdString(msg) + "  ");
}

void MainWindow::periodicUpdate()
{
    ui->pushButtonStartServer->setEnabled(controller->isStartServerButtonEnabled());
    ui->pushButtonStopServer->setEnabled(controller->isStopServerButtonEnabled());
    ui->lineEditPort->setEnabled(controller->isPortLineEditEnabled());
}

void MainWindow::updateInboundData(std::string data)
{
    ui->textEditInboundData->clear();
    ui->textEditInboundData->setText(QString::fromStdString(data));
}

void MainWindow::clientConnectionStatusChange(bool isConnected)
{
    ui->lineEditPeriodicity->setEnabled(!isConnected);
    ui->radioButtonLittleEndian->setEnabled(!isConnected);
    ui->radioButtonBigEndian->setEnabled(!isConnected);

    if(isConnected)
    {
        if(setTransmissionTimer())
        {
            transmissionTimer.start();
        }
    }
    else
    {
        transmissionTimer.stop();
    }
}

void MainWindow::onActionExitTriggered()
{
    close();
}

void MainWindow::onPushButtonStartServerClicked()
{
    if(setPort())
    {
        controller->startServer(ui->lineEditPort->text().toUInt(), ui->radioButtonBigEndian->isChecked());
    }
}

void MainWindow::onPushButtonStopServerClicked()
{
    controller->stopServer();
}

void MainWindow::transmitOutboundData()
{
    controller->transmitOutboundData(ui->textEditOutboundData->toPlainText().toStdString());
}

void MainWindow::requestUpdatePort()
{
    setPort();
}

void MainWindow::requestUpdateTransmissionInterval()
{
    setTransmissionTimer();
}

bool MainWindow::setPort()
{
    if(controller->verifyPort(ui->lineEditPort->text().toStdString()))
    {
        return true;
    }
    else
    {
        controller->showUserInputErrorMessage("Please enter a valid port number.");
        return false;
    }
}

bool MainWindow::setTransmissionTimer()
{
    if(controller->verifyTransmissionInterval(ui->lineEditPeriodicity->text().toStdString()))
    {
        transmissionTimer.setInterval(ui->lineEditPeriodicity->text().toDouble());
        return true;
    }
    else
    {
        controller->showUserInputErrorMessage("Please enter a valid transmission interval.");
        return false;
    }
}

