#include "socketclient.h"
#include <QDebug>

SocketClient::SocketClient(QObject *parent)
    : QObject(parent)
    , tcpSocket(new QTcpSocket(this))
{
    connect(tcpSocket, &QTcpSocket::readyRead, this, &SocketClient::readServerData);
    connect(tcpSocket, &QTcpSocket::connected, this, &SocketClient::onConnected);
    connect(tcpSocket, &QTcpSocket::disconnected, this, &SocketClient::onDisconnected);
    // connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onSocketError(QAbstractSocket::SocketError)));
}

SocketClient::~SocketClient()
{
    // tcpSocket->disconnectFromHost();
}

int SocketClient::connectToServer(const QString &host, quint16 port)
{
    tcpSocket->connectToHost(host, port);
    qDebug() << "Connecting to" << host << ":" << port;
    // if (tcpSocket->waitForConnected(2000))
    // {
    //     qDebug() << "Connected successfully, socket state:" << tcpSocket->state();
    //     return 1;
    // }
    // else
    // {
    //     qDebug() << "Connection failed:" << tcpSocket->errorString();
    //     return -1;
    // }
    
    return 0;
}

bool SocketClient::sendMessage(const QString &message)
{
    if (tcpSocket->state() != QAbstractSocket::ConnectedState) {
        qDebug() << "Not connected to server";
        return false;
    }

    QByteArray data = message.toUtf8() + "\n"; // 메시지 끝에 개행 추가
    qint64 bytesWritten = tcpSocket->write(data);
    tcpSocket->flush();
    if (bytesWritten == -1) {
        qDebug() << "Failed to send message:" << tcpSocket->errorString();
        return false;
    }
    return true;
}

QString SocketClient::receiveMessage()
{
    qDebug() << "receiveMessage ``````````````````````#1";
    // 마지막으로 수신된 메시지 반환
    QString received = lastReceivedMessage;
    lastReceivedMessage.clear(); // 한 번 읽으면 초기화
    return received;
}

void SocketClient::readServerData()
{
    qDebug() << "readServerData ``````````````````````#1";
    qDebug() << "Bytes available:" << tcpSocket->bytesAvailable();
    lastReceivedMessage = QString::fromUtf8(tcpSocket->readAll()).trimmed();
    qDebug() << "Received from server:" << lastReceivedMessage;
    emit messageReceived(false, lastReceivedMessage);
}

void SocketClient::onConnected()
{
    qDebug() << "Connected to server!";
}

void SocketClient::onDisconnected()
{
    qDebug() << "Disconnected from server!";
}

// void SocketClient::onSocketError(QAbstractSocket::SocketError error)
// {
//     qDebug() << "[client] Socket error:" << error << "-" << tcpSocket->errorString();
// }
