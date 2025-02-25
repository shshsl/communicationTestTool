#ifndef SOCKETSERVER_H
#define SOCKETSERVER_H

#include <QObject>

class SocketServer : public QObject
{
    Q_OBJECT
public:
    explicit SocketServer(QObject *parent = 0);

signals:

public slots:
};

#endif // SOCKETSERVER_H