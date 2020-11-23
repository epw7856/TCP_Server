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
    connect(ui->pushButtonStartTransmission, &QPushButton::clicked, this, &MainWindow::onPushButtonStartTransmissionClicked);
    connect(ui->pushButtonStopTransmission, &QPushButton::clicked, this, &MainWindow::onPushButtonStopTransmissionClicked);

    connect(controller.get(), &MainWindowController::updateUI, this, &MainWindow::periodicUpdate);
    connect(controller.get(), &MainWindowController::sendInboundDataToUI, this, &MainWindow::updateInboundData);
    connect(controller.get(), &MainWindowController::sendStatusBarMessage, this, &MainWindow::showStatusBarMessage);

    connect(&transmissionTimer, &QTimer::timeout, this, &MainWindow::transmitOutboundData);

    connect(ui->lineEditPort, &QLineEdit::returnPressed, this, &MainWindow::requestUpdatePort);
    connect(ui->lineEditPeriodicity, &QLineEdit::returnPressed, this, &MainWindow::requestUpdateTransmissionInterval);

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

void MainWindow::stopTransmissions()
{
    if(transmissionTimer.isActive())
    {
        transmissionTimer.stop();
    }
    controller->outboundTransmissionStopped();

    periodicUpdate();
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
    ui->pushButtonStartTransmission->setEnabled(controller->isStartTransmissionButtonEnabled());
    ui->pushButtonStopTransmission->setEnabled(controller->isStopTransmissionButtonEnabled());
    ui->lineEditPort->setEnabled(controller->isPortLineEditEnabled());
    ui->lineEditPeriodicity->setEnabled(controller->isPeriodicityLineEditEnabled());
    ui->radioButtonLittleEndian->setEnabled(controller->areEndianRadioButtonsEnabled());
    ui->radioButtonBigEndian->setEnabled(controller->areEndianRadioButtonsEnabled());
    ui->textEditOutboundData->setEnabled(controller->isOutboundDataTextEditEnabled());
}

void MainWindow::updateInboundData(std::string data)
{
    ui->textEditInboundData->clear();
    ui->textEditInboundData->setText(QString::fromStdString(data));
}

void MainWindow::onActionExitTriggered()
{
    close();
}

void MainWindow::onPushButtonStartServerClicked()
{
    if(controller->verifyPort(ui->lineEditPort->text().toStdString()))
    {
        controller->startServer(ui->lineEditPort->text().toUInt(), ui->radioButtonBigEndian->isChecked());
    }
}

void MainWindow::onPushButtonStopServerClicked()
{
    stopTransmissions();
    controller->stopServer();
}

void MainWindow::onPushButtonStartTransmissionClicked()
{
    if(controller->verifyTransmissionInterval(ui->lineEditPeriodicity->text().toStdString()))
    {
        transmitOutboundData();
        transmissionTimer.setInterval(ui->lineEditPeriodicity->text().toDouble());
        transmissionTimer.start();
    }

    periodicUpdate();
}

void MainWindow::onPushButtonStopTransmissionClicked()
{
    stopTransmissions();
}

void MainWindow::transmitOutboundData()
{
    controller->transmitOutboundData(ui->textEditOutboundData->toPlainText().toStdString());
}

void MainWindow::requestUpdatePort()
{
    controller->verifyPort(ui->lineEditPort->text().toStdString());
}

void MainWindow::requestUpdateTransmissionInterval()
{
    controller->verifyTransmissionInterval(ui->lineEditPeriodicity->text().toStdString());
}
