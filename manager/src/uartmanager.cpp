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
        emit portClosed(SERIAL_PORT_STATE::SERIAL_PORT_CLOSE);
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

void UartManager::writeSerialData(const QString &data)
{
    if (serialPort->isOpen()) {
        serialPort->write(data.toUtf8());
        emit dataSend(data); // 데이터 전송 후 신호발생.
        qDebug() << "Transmitted:" << data;
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

//// *** >> example. << ***
//// *** >> need a specific protocol.
QByteArray UartManager::createPacket(const QByteArray &payload)
{
   if (payload.size() > 64) {
       qDebug() << "Payload size exceeds maximum limit!";
       return QByteArray();
   }

    // make packet.
    // header + payload size + payload + checksum
    QByteArray packet;
    packet.append(0xAA); // header
    packet.append(static_cast<char>(payload.size()));
    packet.append(payload);

    char checksum = 0;
    for (char byte : payload) {
        checksum ^= byte;   // XOR
    }
    packet.append(checksum);

    return packet;
}

void UartManager::sendPacket(const QByteArray &payload)
{
    QByteArray packet = createPacket(payload);
    if (!packet.isEmpty())
    {
        serialPort->write(packet);
        qDebug() << "Packet sent:" << packet.toHex();
    }
}

void UartManager::receiveData()
{
    QByteArray data = serialPort->readAll();

    // ex) simple packet process
    if (data.startsWith(0xAA))  // check header
    {
        int length = static_cast<unsigned char>(data[1]);
        QByteArray payload = data.mid(2, length); // 페이로드 추출

        char checksum = 0;
        for (char byte : payload) {
            checksum ^= byte;
        }

        if (checksum == data[2 + length])   // certificate checksum
        {
            qDebug() << "Valid packet received:" << payload;
        }
        else
        {
            qDebug() << "Checksum error!";
        }
    }
    else
    {
        qDebug() << "header error!";
    }
}

