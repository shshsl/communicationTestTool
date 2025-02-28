#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H

#include <QObject>
#include <QThread>
#include <QTcpServer>
#include <QTcpSocket>

class WorkerThread : public QObject {
    Q_OBJECT
public:
    explicit WorkerThread(QObject *parent = nullptr);
    ~WorkerThread();

public slots:
    void startServer(int port);
    void connectToServer(const QString &host, int port);
    void stop();

signals:
    void serverMessageReceived(const QString &message);
    void clientMessageReceived(const QString &message);
    void serverStarted(bool success);
    void clientConnected(bool success);

private slots:
    void handleNewConnection();
    void readServerData();
    void readClientData();

private:
    QThread thread;
    QTcpServer server;
    QTcpSocket *clientSocket = nullptr;
    QList<QTcpSocket*> serverClients; // 서버에 연결된 클라이언트 소켓 관리
};

#endif // WORKERTHREAD_H