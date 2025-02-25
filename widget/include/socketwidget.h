#ifndef SOCKETWIDGET_H
#define SOCKETWIDGET_H

#include <QWidget>
#include <QTabWidget>
#include <QAbstractSocket>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QLabel>
#include <QFontMetrics>

#include "manager/include/socketmanager.h"

class SocketWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SocketWidget(QWidget *parent = 0);
    ~SocketWidget();

signals:
    void connected();
    void disconnected();
    void error(QAbstractSocket::SocketError);
    void messageReceived(QString);
    void messageSent(QString);

public slots:

private slots:
    void onConnectButtonClicked();
    void onDisconnectButtonClicked();
    void onSendButtonClicked();
    // void onReceiveButtonClicked();
    void onConnected();
    void onDisconnected();
    // void onReadyRead();
    void onError(QAbstractSocket::SocketError error);
    void onMessageReceived(QString message);
    void onMessageSend(QString message);
    
private:
    SocketManager *socketManager;
    
    void createSetConnectLayout(QGridLayout *parentLayout);
    void createConnectionButton(QGridLayout *parentLayout, const QString &labelText, QPushButton *&pushButton);
    void createSetMessageLayout(QGridLayout *parentLayout, const QString &labelText, QLineEdit *&lineEdit);
    void createSetSendLayout(QGridLayout *parentLayout, const QString &labelText, QPushButton *&pushButton);
    
    int calculateMaxCharacters(QLineEdit *lineEdit, bool isNum = false);
    
private:
    QPushButton *connectButton;
    QPushButton *disconnectButton;
    QLineEdit *messageEdit;
    QPushButton *sendButton;
    QTextEdit *messageView;

    int m_nLayoutRow = 0;
    int m_nLayoutColumn = 0;
};

#endif // SOCKETWIDGET_H
