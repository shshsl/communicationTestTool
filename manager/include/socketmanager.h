#ifndef SOCKETMANAGER_H
#define SOCKETMANAGER_H

#include <QObject>
#include <QDebug>
#include <QDateTime>
// #include <QThread>

#include "communicationenum.h"

#include "manager/socket/socketserver.h"
#include "manager/socket/socketclient.h"

class SocketManager : public QObject
{
    Q_OBJECT

public:
    explicit SocketManager(QObject *parent = nullptr);
    ~SocketManager();
    
    // server mode
    bool startAsServer(int port);
    void stopServer();

    // client mode
    bool startAsClient(const QString &ip, int port);
    void stopClient();

    // common
    bool send(const QString &message);

signals:
    void addClientView(QString clientAddress, QDateTime connectTime);
    void receiveMessageToLog(bool, QString);

public slots:
    void addClientToServer(QTcpSocket *clientSocket);
    void receive(bool isClient, const QString &message);

private:
    void setupServerConnections();
    void clearServerConnections();

private:
    SocketServer* server;
    SocketClient* client;
    bool isServerMode;
    QList<QTcpSocket*> clients;
    // QThread workerThread;
    
};

#endif // SOCKETMANAGER_H
