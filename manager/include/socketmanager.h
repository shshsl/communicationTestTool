#ifndef SOCKETMANAGER_H
#define SOCKETMANAGER_H

#include <QObject>
#include <QDebug>
#include <QDateTime>

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
    void receiveMessageToLog(QString);

public slots:
    void addClientToServer(QTcpSocket *clientSocket);
    void receive(const QString &message);

private:
    void setupConnections();
    void clearConnections();

private:
    SocketServer* server;
    SocketClient* client;
    bool isServerMode;
    QList<QTcpSocket*> clients;
    
};

#endif // SOCKETMANAGER_H
