#include "socketserver.h"
#include <QDebug>

#define MAX_USER_COUNT  10

class NetworkError : public std::runtime_error
{
public:
    explicit NetworkError(const QString& message) 
        : std::runtime_error(message.toStdString()) {}
};

SocketServer::SocketServer(QObject *parent) : QObject(parent),
    tcpServer(new QTcpServer(this))
{
    connect(tcpServer, &QTcpServer::newConnection, this, &SocketServer::handleNewConnection);
//    connect(tcpServer, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onSocketError(QAbstractSocket::SocketError)));
}

SocketServer::~SocketServer()
{
    stopServer();
}

bool SocketServer::startServer(quint16 port)
{
    try
    {
        if (!tcpServer->listen(QHostAddress::Any, port))
        {
            throw NetworkError(tr("Failed to start server: %1")
                .arg(tcpServer->errorString()));
        }
        qDebug() << "Server started on port" << port;
        return true;
    }
    catch (const NetworkError& e)
    {
//        emit error(e.what());
        return false;
    }
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
    if (clients.size() >= MAX_USER_COUNT)
    {
        QTcpSocket *clientSocket = tcpServer->nextPendingConnection();
        clientSocket->disconnectFromHost();
        clientSocket->deleteLater();
        qDebug() << "Connection rejected: Maximum client limit (10) reached";
        return;
    }
    
    QTcpSocket *clientSocket = tcpServer->nextPendingConnection();
    clients.append(clientSocket);
    qDebug() << "Client connected:" << clientSocket->peerAddress().toString();
    qDebug() << "Total clients:" << clients.size();
    // QTimer::singleShot(10, [this]() { sendMessage("⭐⭐ Welcome to the server ! ⭐⭐"); });
    
    connect(clientSocket, &QTcpSocket::readyRead, this, &SocketServer::readClientData);
    connect(clientSocket, &QTcpSocket::disconnected, [this, clientSocket]() {
        clients.removeOne(clientSocket);
        clientSocket->deleteLater();
        qDebug() << "Client disconnected, remaining:" << clients.size();
    });
    emit newClientConnected(clientSocket);

    qDebug() << "New client connected";
    qDebug() << "==================== + ====================";
}

void SocketServer::readClientData()
{
    QTcpSocket *client = qobject_cast<QTcpSocket*>(sender());
    m_sLastReceived = "";
    if (client)
    {
        m_sLastReceived = QString::fromUtf8(client->readAll()).trimmed();
        qDebug() << "Received from client:" << m_sLastReceived;
        
        emit notifyReceiveToManager(true, m_sLastReceived);
    }
}

bool SocketServer::sendMessage(const QString &message)
{
    if (clients.isEmpty()) {
        qDebug() << "No clients connected to send message";
        return false;
    }
    QByteArray data = message.toUtf8() + "\n";
    bool allSent = true;
    for (QTcpSocket *client : clients)
    {
        if (client->state() == QAbstractSocket::ConnectedState)
        {
            qint64 bytesWritten = client->write(data);
            qDebug() << "Sending" << data << "to" << client->peerAddress().toString();
            if (bytesWritten == -1)
            {
                qDebug() << "Write failed:" << client->errorString();
                allSent = false;
            }
            else
            {
                qDebug() << "Wrote" << bytesWritten << "bytes successfully";
            }
            client->flush();
        }
    }
    return allSent;
}

QString SocketServer::receiveMessage()
{
//    // 마지막으로 수신된 메시지 반환 (비동기 처리 후 저장된 값)
//    QString received = m_slastReceivedMessage;
//    m_slastReceivedMessage.clear(); // 한 번 읽으면 초기화
//    return received;
}

// void SocketServer::onSocketError(QAbstractSocket::SocketError error)
// {
//     qDebug() << "[server] Socket error:" << error << "-" << tcpServer->errorString();
// }
