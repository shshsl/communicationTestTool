//#pragma once
#ifndef UARTMANAGER_H
#define UARTMANAGER_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>

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

signals:
    void dataReceived(const QString &data);
    void portClosed();
    void portOpened();

private slots:
    void readSerialData();

private:
    QSerialPort *serialPort;
};

#endif // UARTMANAGER_H
