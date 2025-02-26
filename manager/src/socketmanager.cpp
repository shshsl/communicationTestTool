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
