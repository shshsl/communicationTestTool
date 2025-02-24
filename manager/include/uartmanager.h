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
    
    enum SERIAL_PORT_STATE
    {
        SERIAL_PORT_NONE = 0,
        SERIAL_PORT_OPEN = 1,
        SERIAL_PORT_CLOSE = 2,
    };
    Q_ENUM(SERIAL_PORT_STATE)

    void openSerialPort(const QString &portName, int baudRate, QSerialPort::DataBits dataBits,
                        QSerialPort::Parity parity, QSerialPort::StopBits stopBits,
                        QSerialPort::FlowControl flowControl);
    void closeSerialPort();
    QStringList getAvailablePorts() const;

signals:
    void dataReceived(const QString &data);
    void portClosed(int state);
    void portOpened(int state);
    void portFailedToOpen(int state);

private slots:
    void readSerialData();
    void writeSerialData(const QString &data);

private:
    QSerialPort *serialPort;
};

#endif // UARTMANAGER_H
