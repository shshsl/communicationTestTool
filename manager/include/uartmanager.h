//#pragma once
#ifndef UARTMANAGER_H
#define UARTMANAGER_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>

#include "communicationenum.h"

class UartManager : public QObject
{
    Q_OBJECT
public:
    explicit UartManager(QObject *parent = nullptr);
    ~UartManager();

    void openSerialPort(const QString &portName, int baudRate, QSerialPort::DataBits dataBits,
                        QSerialPort::Parity parity, QSerialPort::StopBits stopBits,
                        QSerialPort::FlowControl flowControl);
    void closeSerialPort();
    QStringList getAvailablePorts() const;
    void writeSerialData(const QString &data);

signals:
    void dataReceived(const QString &data);
    void dataSend(const QString &data);
    void portClosed(int state);
    void portOpened(int state);
    void portFailedToOpen(int state);

private slots:
    void readSerialData();

private:
QByteArray createPacket(const QByteArray &payload);
    void sendPacket(const QByteArray &payload);
    void receiveData();

private:
    QSerialPort *serialPort;
};

#endif // UARTMANAGER_H
