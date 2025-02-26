#include "socketserver.h"
#include <QDebug>

SocketServer::SocketServer(QObject *parent) : QObject(parent),
    tcpServer(new QTcpServer(this))
{
    connect(tcpServer, &QTcpServer::newConnection, this, &SocketServer::handleNewConnection);
    
    if (!tcpServer->listen(QHostAddress::Any, 8080))
    { // 포트 12345 예시
        qDebug() << "Server failed to start:" << tcpServer->errorString();
    }
}

SocketServer::~SocketServer()
{
    stopServer();
}

int SocketServer::startServer(quint16 port)
{
    if (!tcpServer->listen(QHostAddress::Any, port))
    {
        qDebug() << "Server failed to start:" << tcpServer->errorString();
        return 0;
    }
    qDebug() << "Server started on port" << port;
    return 1;
}

void SocketServer::stopServer()
{
    for (QTcpSocket *client : clients)
    {
        client->disconnectFromHost();
        delete client;
    }
    tcpServer->close();
}

void SocketServer::testFunction()
{
    
}

void SocketServer::handleNewConnection()
{
    QTcpSocket *clientSocket = tcpServer->nextPendingConnection();
    clients.append(clientSocket);
    connect(clientSocket, &QTcpSocket::readyRead, this, &SocketServer::readClientData);
    connect(clientSocket, &QTcpSocket::disconnected, [this, clientSocket]() {
        clients.removeOne(clientSocket);
        clientSocket->deleteLater();
    });
    qDebug() << "New client connected";
}

void SocketServer::readClientData()
{
    QTcpSocket *client = qobject_cast<QTcpSocket*>(sender());
    if (client)
    {
        lastReceivedMessage = QString::fromUtf8(client->readAll()).trimmed();
        qDebug() << "Received from client:" << lastReceivedMessage;
    }
}

bool SocketServer::sendMessage(const QString &message)
{
    if (clients.isEmpty()) {
        qDebug() << "No clients connected to send message";
        return false;
    }

    QByteArray data = message.toUtf8() + "\n"; // 메시지 끝에 개행 추가
    for (QTcpSocket *client : clients)
    {
        if (client->state() == QAbstractSocket::ConnectedState)
        {
            qint64 bytesWritten = client->write(data);
            client->flush(); // 버퍼 비우기
            if (bytesWritten == -1)
            {
                qDebug() << "Failed to send message to client:" << client->errorString();
                return false;
            }
        }
    }
    return true;
}

QString SocketServer::receiveMessage()
{
    // 마지막으로 수신된 메시지 반환 (비동기 처리 후 저장된 값)
    QString received = lastReceivedMessage;
    lastReceivedMessage.clear(); // 한 번 읽으면 초기화
    return received;
}
