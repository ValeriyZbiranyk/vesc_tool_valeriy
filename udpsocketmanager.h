#ifndef UDPSOCKETMANAGER_H
#define UDPSOCKETMANAGER_H

#include <QObject>
#include <QUdpSocket>

class UdpSocketManager : public QObject
{
    Q_OBJECT
public:
    explicit UdpSocketManager(QObject *parent = nullptr);

private:
    QUdpSocket* udpSocket;

    double leftMotorDuty = 0;
    double rightMotorDuty = 0;

private slots:
    void readPendingDatagrams();

signals:
    void sendControl(const double &leftMotorDuty, const double &rightMotorDuty);
};

#endif // UDPSOCKETMANAGER_H
