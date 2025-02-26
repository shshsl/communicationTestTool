#include "socketclient.h"
#include <QDebug>

SocketClient::SocketClient(QObject *parent)
    : QObject(parent), socket(new QTcpSocket(this))
{
    connect(socket, &QTcpSocket::connected, this, &SocketClient::onConnected);
    connect(socket, &QTcpSocket::disconnected, this, &SocketClient::onDisconnected);
    connect(socket, &QTcpSocket::readyRead, this, &SocketClient::readServerData);
    connect(socket, &QTcpSocket::errorOccurred, [](QAbstractSocket::SocketError socketError) {
        qDebug() << "Socket error:" << socketError;
    });
}

SocketClient::~SocketClient()
{
    socket->disconnectFromHost();
}

void SocketClient::connectToServer(const QString &host, quint16 port)
{
    // ex) option: QHostAddress::LocalHost, 192.168.1.1
    socket->connectToHost(host, port);
}

void SocketClient::sendMessage(const QString &message)
{
    if (socket->state() == QAbstractSocket::ConnectedState) {
        socket->write(message.toUtf8());
    } else {
        qDebug() << "Not connected to server!";
    }
}

void SocketClient::onConnected()
{
    qDebug() << "Connected to server!";
}

void SocketClient::onDisconnected()
{
    qDebug() << "Disconnected from server!";
}

void SocketClient::readServerData()
{
    QByteArray data = socket->readAll();
    qDebug() << "Server response:" << data;
}