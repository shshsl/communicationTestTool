#include "manager/include/uartmanager.h"
#include <QDebug>

UartManager::UartManager(QObject *parent)
    : QObject(parent), serialPort(new QSerialPort(this))
{
    connect(serialPort, &QSerialPort::readyRead, this, &UartManager::readSerialData);
}

UartManager::~UartManager()
{
    if (serialPort->isOpen())
        serialPort->close();
}

void UartManager::openSerialPort(const QString &portName, int baudRate,
                                 QSerialPort::DataBits dataBits, QSerialPort::Parity parity,
                                 QSerialPort::StopBits stopBits, QSerialPort::FlowControl flowControl)
{
    if (serialPort->isOpen())
        serialPort->close();

    serialPort->setPortName(portName);
    serialPort->setBaudRate(baudRate);
    serialPort->setDataBits(dataBits);
    serialPort->setParity(parity);
    serialPort->setStopBits(stopBits);
    serialPort->setFlowControl(flowControl);
    
    qDebug() << "set port options..." << portName;

    if (serialPort->open(QIODevice::ReadWrite))
    {
        emit portOpened(SERIAL_PORT_STATE::SERIAL_PORT_OPEN);
        qDebug() << "Opened port:" << portName;
    }
    else
    {
        emit portFailedToOpen(SERIAL_PORT_STATE::SERIAL_PORT_NONE);
        qDebug() << "Failed to open port:" << portName;
        qDebug() << "Error:" << serialPort->errorString();
    }
}

void UartManager::closeSerialPort()
{
    if (serialPort->isOpen())
    {
        serialPort->close();
        emit portClosed(SERIAL_PORT_STATE::SERIAL_PORT_NONE);
        qDebug() << "Closed port.";
    }
}

void UartManager::readSerialData()
{
    while (serialPort->canReadLine())
    {
        QByteArray data = serialPort->readLine();
        QString logEntry = QString::fromUtf8(data).trimmed();
        emit dataReceived(logEntry); // 데이터를 시그널로 전송
        qDebug() << "Received:" << logEntry;
    }
}

QStringList UartManager::getAvailablePorts() const
{
    QStringList ports;
    const auto availablePorts = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &port : availablePorts)
        ports.append(port.portName());
    
    return ports;
}
