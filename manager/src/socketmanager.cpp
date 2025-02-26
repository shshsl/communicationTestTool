#include "manager/include/socketmanager.h"

SocketManager::SocketManager(QObject *parent) : QObject(parent), 
    server(nullptr),
    client(nullptr),
    isServerMode(false)
{
    setupConnections();
}

SocketManager::~SocketManager()
{
    clearConnections();
    if (server) delete server;
    if (client) delete client;
}

void SocketManager::setupConnections()
{
    connect(server, &SocketServer::notifyReceiveToManager, this, &SocketManager::receive);
}

void SocketManager::clearConnections()
{
    
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
    qDebug() << "** message from what ?=  " << isServerMode << " * " << server << " * " << client << " * " << message;
    
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

void SocketManager::receive(const QString &message)
{
    qDebug() << "** message to widget, what ?=" << message;
    
    // QString rMessage = "";
    // if (isServerMode && server)
    // {
    //     rMessage = server->receiveMessage();
    // }
    // else if (!isServerMode && client)
    // {
    //     rMessage = client->receiveMessage();
    // }
    
    emit receiveMessageToLog(message);
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
