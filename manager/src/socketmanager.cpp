#include "manager/include/socketmanager.h"

SocketManager::SocketManager(QObject *parent) : QObject(parent), 
    server(nullptr),
    client(nullptr),
    isServerMode(false)
{
    
}

SocketManager::~SocketManager() {
    if (server) delete server;
    if (client) delete client;
}

bool SocketManager::startAsServer(int port)
{
    server = new SocketServer();
    isServerMode = true;
    connect(server, &SocketServer::newClientConnected, this, &SocketManager::addClientToServer);
    
    return server->startServer(port);
}

void SocketManager::stopServer()
{
   if (server) {
       server->stopServer();
       delete server;
       server = nullptr;
   }
   isServerMode = false;
}

bool SocketManager::startAsClient(const QString &ip, int port)
{
    client = new SocketClient();
    isServerMode = false;
    return client->connectToServer(ip, port);
}

void SocketManager::stopClient()
{
    if (client) {
        client->disconnect();
        delete client;
        client = nullptr;
    }
}

bool SocketManager::send(const QString &message)
{
    if (isServerMode && server)
    {
        return server->sendMessage(message);
    }
    else if (!isServerMode && client)
    {
        return client->sendMessage(message);
    }
    return false;
}

QString SocketManager::receive()
{
    if (isServerMode && server)
    {
        return server->receiveMessage();
    }
    else if (!isServerMode && client)
    {
        return client->receiveMessage();
    }
    return "";
}

void SocketManager::addClientToServer(QTcpSocket *clientSocket)
{
    clients.append(clientSocket);
    QHostAddress clientAddr = clientSocket->peerAddress();
    QString convertStr = "";
    if (clientAddr.protocol() == QAbstractSocket::IPv4Protocol)
    {
        convertStr = QHostAddress(clientAddr.toIPv4Address()).toString();
    }
    else
    {
        if (clientAddr.toString().startsWith("::ffff:"))
        {
            convertStr = clientAddr.toString().mid(7); // remove - "::ffff:"
        }
        else
        {
            convertStr = clientAddr.toString(); // IPv6 Address
        }
    }
    qDebug() << "** add a client :  " << convertStr;

    connect(clientSocket, &QTcpSocket::disconnected, this, [this, clientSocket]() {
        clients.removeOne(clientSocket);
        clientSocket->deleteLater();
        qDebug() << "** remove a client.";
    });

    emit addClientView(convertStr, QDateTime::currentDateTime());
}
