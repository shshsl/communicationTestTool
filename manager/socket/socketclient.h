#ifndef SOCKETCLIENT_H
#define SOCKETCLIENT_H

#include <QObject>
#include <QTcpSocket>

class SocketClient : public QObject {
    Q_OBJECT

public:
    explicit SocketClient(QObject *parent = nullptr);
    ~SocketClient();
    
    void connectToServer(const QString &host, quint16 port);
    void sendMessage(const QString &message);

private slots:
    void onConnected();
    void onDisconnected();
    void readServerData();

private:
    QTcpSocket *socket;
};

#endif // SOCKETCLIENT_H