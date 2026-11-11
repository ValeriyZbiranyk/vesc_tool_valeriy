#ifndef UDPSOCKETMANAGER_H
#define UDPSOCKETMANAGER_H

#include <QObject>
#include <QUdpSocket>
#include <QTimer>

class UdpSocketManager : public QObject
{
    Q_OBJECT
public:
    explicit UdpSocketManager(QObject *parent = nullptr);

    bool isClientOnline() { return isClientAlive; }

private:
    enum TelemetryDataType {
        Battery = 0,
        DriversState = 1,
        Coordinates = 2
    };

    struct TelemetryData {
        TelemetryDataType currentType= TelemetryDataType::Battery;
        int batteryValue = 0;
        bool driversState = 0;
        QString mgrsCoordinates = "";

        char getCurrentType() { return (char)currentType; }

        QByteArray getCurrentValue() {
            QByteArray val;

            switch (currentType) {
            case TelemetryDataType::Battery:
                val.append((char)batteryValue);
                break;
            case TelemetryDataType::DriversState:
                val.append((char)driversState);
                break;
            case TelemetryDataType::Coordinates:
                val.append(mgrsCoordinates);
                break;
            default:
                break;
            }

            return val;
        }

        void setNextType() {
            TelemetryDataType nextType = TelemetryDataType::Battery;

            switch (currentType){
            case TelemetryDataType::Battery:
                currentType = TelemetryDataType::DriversState;
                break;
            case TelemetryDataType::DriversState:
                currentType = TelemetryDataType::Coordinates;
                break;
            case TelemetryDataType::Coordinates:
                currentType = TelemetryDataType::Battery;
                break;
            default:
                break;
            }

            currentType = nextType;
        }
    };

    QUdpSocket* udpSocket;

    double leftMotorDuty = 0;
    double rightMotorDuty = 0;
    TelemetryData telemetryData;
    QTimer* clientTimeoutTimer;
    bool isClientAlive = false;


public slots:
    void receiveTelemetry(const int &_batteryValue, const bool &_driversState, const QString &_mgrsCoordinates);

private slots:
    void readPendingDatagrams();


signals:
    void sendControl(const double &_leftMotorDuty, const double &_rightMotorDuty);
};

#endif // UDPSOCKETMANAGER_H
