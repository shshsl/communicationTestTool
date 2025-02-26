#ifndef SOCKETMANAGER_H
#define SOCKETMANAGER_H

#include <QObject>
#include <QDebug>

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
    QString receive();

signals:

public slots:

private:
    SocketServer* server;
    SocketClient* client;
    bool isServerMode;
    
};

#endif // SOCKETMANAGER_H
