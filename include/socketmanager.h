#ifndef SOCKETMANAGER_H
#define SOCKETMANAGER_H

#include <QObject>

class SocketManager : public QObject
{
    Q_OBJECT
public:
    explicit SocketManager(QObject *parent = 0);

signals:

public slots:
};

#endif // SOCKETMANAGER_H