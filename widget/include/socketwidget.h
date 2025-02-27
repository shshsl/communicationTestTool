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
#include <QDebug>
#include <QHostAddress> // IP 유효성 검사에 사용

#include "manager/include/socketmanager.h"
#include "communicationenum.h"

class SocketWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SocketWidget(QWidget *parent = 0);
    ~SocketWidget();

    // void addClient(const QString &ip, const QDateTime &connectTime);
    void setupServer(int port);
    void setupClient(const QString &ip, int port);
    void sendMessage(const QString &message);
    void receiveMessage();

signals:
    void connected();
    void disconnected();
    void error(QAbstractSocket::SocketError);
    void messageReceived(QString);
    void messageSent(QString);

public slots:
    // void updateClientList(QString clientAddress);
    void addClient(const QString &ip, const QDateTime &connectTime);

private slots:
    void onOptionButtonClicked();
    void updateElapsedTime();

private:
    SocketManager *socketManager;
    
    void createTabWidget(QGridLayout *parentLayout);
    void createOptionLayout(QGridLayout *parentLayout);
    void createClientsView(QGridLayout *parentLayout);
    void createFrameBox();
    
    int resizeWidthForEdit(QLineEdit *lineEdit, Communication::Socket::ConnectOption option);
    void testFunction();
    
private:
    QTabWidget *socketTabWidget;
    QWidget *serverTab;
    QWidget *clientTab;
    QWidget *udpTab;
    QGridLayout *serverLayout;
    QGridLayout *clientLayout;
    QGridLayout *udpLayout;
    
    QLineEdit *ipEdit;
    QLineEdit *portEdit;
    QPushButton *optionPushButton;
    QPushButton *connectButton;
    QPushButton *disconnectButton;
    QLineEdit *messageEdit;
    QPushButton *sendButton;
    QTextEdit *messageView;

    int m_nCurrentTab = 0;
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
    
    QFrame *frame;
};

#endif // SOCKETWIDGET_H
