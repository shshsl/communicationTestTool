#ifndef SOCKETSERVER_H
#define SOCKETSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>
#include <QDateTime>
#include <QDebug>

#include "widget/include/socketwidget.h"

class SocketServer : public QObject {
    Q_OBJECT

public:
    explicit SocketServer(QObject *parent = nullptr);
    ~SocketServer();
    
    void startServer(quint16 port);

private slots:
    void handleNewConnection();
    void readClientData();

private:
    SocketWidget *socketWidget;

    QTcpServer *server;
    QList<QTcpSocket*> clients;
};

#endif // SOCKETSERVER_H
