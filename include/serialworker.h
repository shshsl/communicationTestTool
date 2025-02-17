#ifndef SERIALWORKER_H
#define SERIALWORKER_H

#include <QObject>
#include <QProgressDialog>
#include <QThread>
#include <QMessageBox>
#include <QSerialPort>

class SerialWorker : public QObject {
    Q_OBJECT
public:
    explicit SerialWorker(QSerialPort* port) : serialPort(port) {}

public slots:
    void openPort()
    {
//        bool result = serialPort->open(QIODevice::ReadOnly);
//        emit portOpened(result);
    }

signals:
    void portOpened(bool success);

private:
    QSerialPort* serialPort;
};

#endif // SERIALWORKER_H
