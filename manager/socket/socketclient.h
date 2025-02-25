#ifndef SOCKETCLIENT_H
#define SOCKETCLIENT_H

#include <QObject>

class SocketClient : public QObject
{
    Q_OBJECT
public:
    explicit SocketClient(QObject *parent = 0);

signals:

public slots:
};

#endif // SOCKETCLIENT_H