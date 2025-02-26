#ifndef SOCKETSERVER_H
#define SOCKETSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>

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
    QTcpServer *server;
    QList<QTcpSocket*> clients;
};

#endif // SOCKETSERVER_H