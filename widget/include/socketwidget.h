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
#include <QStyleOptionFrame>
#include <QTimer>
#include <QDateTime>

#include "manager/include/socketmanager.h"
#include "communicationenum.h"

class SocketWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SocketWidget(QWidget *parent = 0);
    ~SocketWidget();

    void addClient(const QString &ip, const QDateTime &connectTime);

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
    
    void updateElapsedTime();

private:
    SocketManager *socketManager;
    
    void createTabWidget(QGridLayout *parentLayout);
    void createOptionLayout(QGridLayout *parentLayout);
    void createConnectionButton(QGridLayout *parentLayout, const QString &labelText, QPushButton *&pushButton);
    
    void createClientsView(QGridLayout *parentLayout);
    
    void createSetMessageLayout(QGridLayout *parentLayout, const QString &labelText, QLineEdit *&lineEdit);
    void createSetSendLayout(QGridLayout *parentLayout, const QString &labelText, QPushButton *&pushButton);
    
    int resizeWidthForEdit(QLineEdit *lineEdit, Communication::Socket::ConnectOption option);
    
private:
    QTabWidget *socketTabWidget;
    QWidget *serverTab;
    QWidget *clientTab;
    QWidget *udpTab;
    QGridLayout *serverLayout;
    QGridLayout *clientLayout;
    QGridLayout *udpLayout;
    
    QPushButton *connectButton;
    QPushButton *disconnectButton;
    QLineEdit *messageEdit;
    QPushButton *sendButton;
    QTextEdit *messageView;

    int m_nLayoutRow = 0;
    int m_nLayoutColumn = 0;
    
    //server (client view)
    QLabel *ipLabel;
    QLabel *timeLabel;
    QDateTime startTime;
    QTimer *timer;
    
    struct ClientInfo
    {
        QString ipAddress;
        QDateTime connectTime;
        QLabel *timeLabel; // 실시간 업데이트를 위한 라벨
        QTimer *timer;     // 클라이언트별 타이머
    };

    QHBoxLayout *clientsLayout;
    QList<ClientInfo> clients;
    
};

#endif // SOCKETWIDGET_H
