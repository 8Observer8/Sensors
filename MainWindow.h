#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QList>
#include <QTimer>
#include <QTcpSocket>
#include "Sensor.h"

QT_BEGIN_NAMESPACE
class QNetworkSession;
QT_END_NAMESPACE

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void sendToServer(const QList<Sensor> &sensorList);

private slots:
    void on_AddAction_triggered();

    void on_EditAction_triggered();

    void slotAddSensor(QString sensorName, int portNumber);

    void on_ConnectAction_triggered();

    void on_ViewConnectionAction_triggered();

    void setSettinsOfConnection(QString hostName, int portNumber);

    void slotSendSensorValues();

    void sessionOpened();

    void displayError(QAbstractSocket::SocketError socketError);

private:
    Ui::MainWindow *ui;
    QStandardItemModel *mModel;
    QList<Sensor> mSensors;
    QString mHostName;
    int mPortNumber;
    QTimer *mTimer;
    QNetworkSession *mNetworkSession;
    QTcpSocket *mTcpSocket;
};

#endif // MAINWINDOW_H
