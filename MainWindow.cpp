#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "AddDialog.h"
#include "EditDialog.h"
#include "SettingDialog.h"
#include <QLabel>
#include <QMessageBox>
#include <QtNetwork>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mHostName(""),
    mPortNumber(0)
{
    ui->setupUi(this);

    mTimer = new QTimer(this);
    mTimer->start(1000);
    connect(mTimer, SIGNAL(timeout()), this, SLOT(slotSendSensorValues()));

    int rows = 0;
    int columns = 3;
    mModel = new QStandardItemModel(rows, columns, this);
    mModel->setHeaderData(0, Qt::Horizontal, QString("Sensor"));
    mModel->setHeaderData(1, Qt::Horizontal, QString("Value Of Sensor"));
    mModel->setHeaderData(2, Qt::Horizontal, QString("Name of Port"));
    ui->table->setModel(mModel);

    QNetworkConfigurationManager manager;
    if (manager.capabilities() & QNetworkConfigurationManager::NetworkSessionRequired) {
        // Get saved network configuration
        QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
        settings.beginGroup(QLatin1String("QtNetwork"));
        const QString id = settings.value(QLatin1String("DefaultNetworkConfiguration")).toString();
        settings.endGroup();

        // If the saved network configuration is not currently discovered use the system default
        QNetworkConfiguration config = manager.configurationFromIdentifier(id);
        if ((config.state() & QNetworkConfiguration::Discovered) !=
                QNetworkConfiguration::Discovered) {
            config = manager.defaultConfiguration();
        }

        mNetworkSession = new QNetworkSession(config, this);
        connect(mNetworkSession, SIGNAL(opened()), this, SLOT(sessionOpened()));
        mNetworkSession->open();
    }
}

void MainWindow::sessionOpened()
{
    // Save the used configuration
    QNetworkConfiguration config = mNetworkSession->configuration();
    QString id;
    if (config.type() == QNetworkConfiguration::UserChoice)
        id = mNetworkSession->sessionProperty(QLatin1String("UserChoiceConfiguration")).toString();
    else
        id = config.identifier();

    QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
    settings.beginGroup(QLatin1String("QtNetwork"));
    settings.setValue(QLatin1String("DefaultNetworkConfiguration"), id);
    settings.endGroup();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::sendToServer(const QList<Sensor> &sensorList)
{
    QByteArray  arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_7);

    QString sensorsInfo = QString("currentValues,0;");
    for (int i = 0; i < sensorList.size(); ++i) {
        sensorsInfo += sensorList[i].sensorName() + "," + QString::number(sensorList[i].value());
        if (i != sensorList.size()-1) {
            sensorsInfo += ";";
        }
    }

    out << quint16(0) << sensorsInfo;

    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    mTcpSocket->write(arrBlock);
}

void MainWindow::on_AddAction_triggered()
{
    AddDialog *dialog = new AddDialog(this);
    dialog->setModal(true);
    connect(dialog, SIGNAL(signalNameAndNumber(QString, int)), this, SLOT(slotAddSensor(QString, int)));
    dialog->show();
}

void MainWindow::on_EditAction_triggered()
{
    EditDialog *dialog = new EditDialog(this);
    dialog->setModal(true);
    connect(dialog, SIGNAL(signalNameAndNumber(QString, int)), this, SLOT(slotAddSensor(QString, int)));
    dialog->show();
}

void MainWindow::slotAddSensor(QString sensorName, int portNumber)
{
    // Create Sensor
    QString portName = QString("COM") + QString::number(portNumber);
    Sensor sensor(sensorName, portName);
    mSensors.push_back(sensor);

    // Add Sensor to the table
    QList<QStandardItem*> items;
    QStandardItem *item = new QStandardItem(sensorName);
    items.push_back(item);

    item = new QStandardItem(QString::number(sensor.value()));
    items.push_back(item);

    item = new QStandardItem(portName);
    items.push_back(item);
    mModel->appendRow(items);
}

void MainWindow::on_ConnectAction_triggered()
{
    SettingDialog *dialog = new SettingDialog(this);
    dialog->setModal(true);
    connect(dialog, SIGNAL(signalHostNameAndPortNumber(QString,int)), this, SLOT(setSettinsOfConnection(QString,int)));
    dialog->show();
}

void MainWindow::on_ViewConnectionAction_triggered()
{
    QMessageBox::information(this, QString("Information"), QString("Host Name: %1\nPort Number: %2").arg(mHostName).arg(mPortNumber));
}

void MainWindow::setSettinsOfConnection(QString hostName, int portNumber)
{
    mHostName = hostName;
    mPortNumber = portNumber;

    mTcpSocket = new QTcpSocket(this);

    connect(mTcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(displayError(QAbstractSocket::SocketError)));

    mTcpSocket->connectToHost(hostName, portNumber);
}

void MainWindow::slotSendSensorValues()
{
    // Set data in the table and send to server
    QModelIndex index;
    for (int i = 0; i < mSensors.size(); ++i) {
        // Set data to the table
        index = mModel->index(i, 1);
        mModel->setData(index, mSensors[i].value());

        // Send to server
        if (!mHostName.isEmpty()) {
            sendToServer(mSensors);
        }
    }
}

void MainWindow::displayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        QMessageBox::information(this, tr("Sensors Error"),
                                 tr("The host was not found. Please check the "
                                    "host name and port settings."));
        break;
    case QAbstractSocket::ConnectionRefusedError:
        QMessageBox::information(this, tr("Sensors Error"),
                                 tr("The connection was refused by the peer. "
                                    "Make sure the server is running, "
                                    "and check that the host name and port "
                                    "settings are correct."));
        break;
    default:
        QMessageBox::information(this, tr("Sensors Error"),
                                 tr("The following error occurred: %1.")
                                 .arg(mTcpSocket->errorString()));
    }
}

void MainWindow::on_ExitAction_triggered()
{
    close();
}
