#include "udpsocketmanager.h"

#include <QNetworkDatagram>

UdpSocketManager::UdpSocketManager(QObject *parent)
    : QObject{parent}
{
    udpSocket = new QUdpSocket(this);
    udpSocket->bind(QHostAddress::AnyIPv4, 7775);
    connect(udpSocket, &QUdpSocket::readyRead, this, &UdpSocketManager::readPendingDatagrams);
}

void UdpSocketManager::readPendingDatagrams()
{
    while(udpSocket->hasPendingDatagrams()) {
        QNetworkDatagram datagram = udpSocket->receiveDatagram();
        qDebug() << datagram.data();
        udpSocket->writeDatagram(datagram.makeReply(QByteArray("Sure its work, i`m fucking cool!")));
    }
}
