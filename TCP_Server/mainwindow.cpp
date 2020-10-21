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
    connect(controller.get(), &MainWindowController::sendStatusBarMessage, this, &MainWindow::showStatusBarMessage);

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

}

void MainWindow::onActionExitTriggered()
{
    close();
}

void MainWindow::onPushButtonStartServerClicked()
{
    controller->startServer(ui->lineEditPort->text().toUInt(), ui->radioButtonBigEndian->isChecked());
}

void MainWindow::onPushButtonStopServerClicked()
{
    controller->stopServer();
}

