#ifndef SOCKETSERVER_H
#define SOCKETSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>
#include <QDateTime>
#include <QDebug>

class SocketServer : public QObject {
    Q_OBJECT

public:
    explicit SocketServer(QObject *parent = nullptr);
    ~SocketServer();
    
    int startServer(quint16 port);
    void stopServer();
    
    bool sendMessage(const QString &message);
    QString receiveMessage();

signals:
    void newClientConnected(QString &clientIp, QDateTime &connectTime);

private slots:
    void handleNewConnection();
    void readClientData();

private:
    void testFunction();

private:
    QTcpServer *tcpServer;
    QList<QTcpSocket*> clients;
    QString lastReceivedMessage;
    
};

#endif // SOCKETSERVER_H
