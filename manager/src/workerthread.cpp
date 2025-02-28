#include "manager/include/workerthread.h"
#include <QDebug>

WorkerThread::WorkerThread(QObject *parent) : QObject(parent)
{
    server.moveToThread(&thread);
    if (clientSocket) clientSocket->moveToThread(&thread);
    thread.start();
}

WorkerThread::~WorkerThread()
{
    stop();
    thread.quit();
    thread.wait();
}

void WorkerThread::startServer(int port)
{
    connect(&server, &QTcpServer::newConnection, this, &WorkerThread::handleNewConnection, Qt::UniqueConnection);
    bool success = server.listen(QHostAddress::Any, port);
    emit serverStarted(success);
    if (!success) qDebug() << "Server failed to start!";
}

void WorkerThread::connectToServer(const QString &host, int port)
{
    if (!clientSocket) {
        clientSocket = new QTcpSocket(this);
        clientSocket->moveToThread(&thread);
        connect(clientSocket, &QTcpSocket::readyRead, this, &WorkerThread::readClientData);
        connect(clientSocket, &QTcpSocket::connected, this, [this]() { emit clientConnected(true); });
//        connect(clientSocket, &QTcpSocket::error, this, [this]() { emit clientConnected(false); });
    }
    clientSocket->connectToHost(host, port);
}

void WorkerThread::stop()
{
    if (clientSocket) {
        clientSocket->disconnectFromHost();
        clientSocket->deleteLater();
        clientSocket = nullptr;
    }
    server.close();
    qDeleteAll(serverClients);
    serverClients.clear();
}

void WorkerThread::handleNewConnection()
{
    QTcpSocket *client = server.nextPendingConnection();
    serverClients.append(client);
    connect(client, &QTcpSocket::readyRead, this, &WorkerThread::readServerData);
    connect(client, &QTcpSocket::disconnected, this, [this, client]() {
        serverClients.removeOne(client);
        client->deleteLater();
    });
}

void WorkerThread::readServerData()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if (socket) {
        QString message = socket->readAll();
        emit serverMessageReceived(message);
    }
}

void WorkerThread::readClientData()
{
    if (clientSocket) {
        QString message = clientSocket->readAll();
        emit clientMessageReceived(message);
    }
}

