#ifndef SOCKETCLIENT_H
#define SOCKETCLIENT_H

#include <QObject>
#include <QTcpSocket>

class SocketClient : public QObject
{
    Q_OBJECT

public:
    explicit SocketClient(QObject *parent = nullptr);
    ~SocketClient();
    
    bool sendMessage(const QString &message);
    QString receiveMessage();
    int connectToServer(const QString &host, quint16 port);

signals:
    void messageReceived(bool, const QString &message);

private slots:
    void onConnected();
    void onDisconnected();
    void readServerData();
    // void onSocketError(QAbstractSocket::SocketError error);

private:
    QTcpSocket *tcpSocket;
    QString lastReceivedMessage;
};

#endif // SOCKETCLIENT_H