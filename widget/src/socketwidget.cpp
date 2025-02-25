#include "widget/include/socketwidget.h"

SocketWidget::SocketWidget(QWidget *parent) 
    : QWidget(parent)//, socketManager(new SocketManager(this))
{
    // connect(socketManager, SIGNAL(connected()), this, SLOT(onConnected()));
    // connect(socketManager, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
    // connect(socketManager, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onError(QAbstractSocket::SocketError)));
    // connect(socketManager, SIGNAL(messageReceived(QString)), this, SLOT(onMessageReceived(QString)));
    // connect(socketManager, SIGNAL(messageSent(QString)), this, SLOT(onMessageSend(QString)));

    QVBoxLayout *layout = new QVBoxLayout(this);
    setLayout(layout);

    connectButton = new QPushButton("Connect", this);
    layout->addWidget(connectButton);
    connect(connectButton, SIGNAL(clicked()), this, SLOT(onConnectButtonClicked()));

    disconnectButton = new QPushButton("Disconnect", this);
    layout->addWidget(disconnectButton);
    connect(disconnectButton, SIGNAL(clicked()), this, SLOT(onDisconnectButtonClicked()));

    messageEdit = new QLineEdit(this);
    layout->addWidget(messageEdit);

    sendButton = new QPushButton("Send", this);
    layout->addWidget(sendButton);
//    connect(sendButton, SIGNAL(clicked()), this, SLOT(onSendButtonClicked());

    messageView = new QTextEdit(this);
    layout->addWidget(messageView);
}

SocketWidget::~SocketWidget()
{
    
}

void SocketWidget::onConnectButtonClicked()
{
    // socketManager->connectToHost();
}

void SocketWidget::onDisconnectButtonClicked()
{
    // socketManager->disconnectFromHost();
}

void SocketWidget::onSendButtonClicked()
{
    // socketManager->sendMessage(messageEdit->text());
}

void SocketWidget::onConnected()
{
    messageView->append("Connected");
}

void SocketWidget::onDisconnected()
{
    messageView->append("Disconnected");
}

void SocketWidget::onError(QAbstractSocket::SocketError error)
{
    // messageView->append("Error: " + socketManager->errorString());
}

void SocketWidget::onMessageReceived(QString message)
{
    messageView->append("Received: " + message);
}

void SocketWidget::onMessageSend(QString message)
{
    messageView->append("Sent: " + message);
}

