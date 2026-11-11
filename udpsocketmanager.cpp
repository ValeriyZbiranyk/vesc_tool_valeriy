#include "udpsocketmanager.h"

#include <QNetworkDatagram>

UdpSocketManager::UdpSocketManager(QObject *parent)
    : QObject{parent}
{
    udpSocket = new QUdpSocket(this);
    udpSocket->bind(QHostAddress::AnyIPv4, 7775);
    connect(udpSocket, &QUdpSocket::readyRead, this, &UdpSocketManager::readPendingDatagrams);

    clientTimeoutTimer = new QTimer(this);
    clientTimeoutTimer->setInterval(5000);
}

void UdpSocketManager::receiveTelemetry(const int &_batteryValue, const bool &_driversState, const QString &_mgrsCoordinates)
{
    telemetryData.batteryValue = _batteryValue;
    telemetryData.driversState = _driversState;
    telemetryData.mgrsCoordinates = _mgrsCoordinates;
}

void UdpSocketManager::readPendingDatagrams()
{
    while(udpSocket->hasPendingDatagrams()) {
        QNetworkDatagram datagram = udpSocket->receiveDatagram();
        char controll = datagram.data().at(0);

        double leftMotorDuty = (((controll >> 4) & 0x0F) / 7.5) - 1;
        double rightMotorDuty = ((controll & 0x0F) / 7.5) - 1;

        emit sendControl(leftMotorDuty, rightMotorDuty);

        QByteArray reply;
        telemetryData.setNextType();
        reply.push_back(telemetryData.getCurrentType());
        reply.push_back(telemetryData.getCurrentValue());

        udpSocket->writeDatagram(datagram.makeReply(QByteArray(reply)));

        clientTimeoutTimer->stop();
        clientTimeoutTimer->start();
    }
}
