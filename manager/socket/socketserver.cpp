#include "socketserver.h"
#include <QDebug>

SocketServer::SocketServer(QObject *parent)
    : QObject(parent), server(new QTcpServer(this))
{
    connect(server, &QTcpServer::newConnection, this, &SocketServer::handleNewConnection);
}

SocketServer::~SocketServer()
{
    for (QTcpSocket *client : clients) {
        client->disconnectFromHost();
        delete client;
    }
    server->close();
}

void SocketServer::startServer(quint16 port)
{
    if (!server->listen(QHostAddress::Any, port)) {
        qDebug() << "Server failed to start:" << server->errorString();
        return;
    }
    qDebug() << "Server started on port" << port;
}

void SocketServer::handleNewConnection()
{
    QTcpSocket *client = server->nextPendingConnection();
    clients.append(client);
    connect(client, &QTcpSocket::readyRead, this, &SocketServer::readClientData);
    connect(client, &QTcpSocket::disconnected, client, &QTcpSocket::deleteLater);
    qDebug() << "New client connected:" << client->peerAddress().toString();
}

void SocketServer::readClientData()
{
    QTcpSocket *client = qobject_cast<QTcpSocket*>(sender());
    if (!client) return;

    QByteArray data = client->readAll();
    qDebug() << "Received from" << client->peerAddress().toString() << ":" << data;

    // 클라이언트에 응답 (에코 서버 예제)
    client->write("Echo: " + data);
}

