#include "widget/include/socketwidget.h"

SocketWidget::SocketWidget(QWidget *parent) 
    : QWidget(parent), socketManager(new SocketManager(this)),
    m_nLayoutRow(0), m_nLayoutColumn(0)
{
    // UI 생성
    QGridLayout *layout = new QGridLayout(this);
    setLayout(layout);

    QTabWidget *socketTabWidget = new QTabWidget(this);

    // Server 
    QWidget *serverTab = new QWidget();
    QGridLayout *serverLayout = new QGridLayout(serverTab);
    // serverLayout->addWidget(new QLabel("Server Settings"));
    // serverLayout->addWidget(new QLineEdit("Enter server address"));
    // serverLayout->addWidget(new QPushButton("Start Server"));

    // Client
    QWidget *clientTab = new QWidget();
    QGridLayout *clientLayout = new QGridLayout(clientTab);
    // clientLayout->addWidget(new QLabel("Client Settings"));
    // clientLayout->addWidget(new QLineEdit("Enter client address"));
    // clientLayout->addWidget(new QPushButton("Connect to Server"));
    
    QWidget *udpTab = new QWidget();
    QGridLayout *udpLayout = new QGridLayout(udpTab);

    socketTabWidget->addTab(serverTab, "TCP Server");
    socketTabWidget->addTab(clientTab, "TCP Client");
    socketTabWidget->addTab(udpTab, "UDP");
    
    createSetConnectLayout(serverLayout);
    // createSetConnectLayout(clientLayout);

    layout->addWidget(socketTabWidget);
    
    

//     connectButton = new QPushButton("Connect", this);
//     layout->addWidget(connectButton);
//     connect(connectButton, SIGNAL(clicked()), this, SLOT(onConnectButtonClicked()));

//     disconnectButton = new QPushButton("Disconnect", this);
//     layout->addWidget(disconnectButton);
//     connect(disconnectButton, SIGNAL(clicked()), this, SLOT(onDisconnectButtonClicked()));

//     messageEdit = new QLineEdit(this);
//     layout->addWidget(messageEdit);

//     sendButton = new QPushButton("Send", this);
//     layout->addWidget(sendButton);
// //    connect(sendButton, SIGNAL(clicked()), this, SLOT(onSendButtonClicked());

//     messageView = new QTextEdit(this);
//     layout->addWidget(messageView);

    // connect(socketManager, SIGNAL(connected()), this, SLOT(onConnected()));
    // connect(socketManager, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
    // connect(socketManager, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onError(QAbstractSocket::SocketError)));
    // connect(socketManager, SIGNAL(messageReceived(QString)), this, SLOT(onMessageReceived(QString)));
    // connect(socketManager, SIGNAL(messageSent(QString)), this, SLOT(onMessageSend(QString)));
}

SocketWidget::~SocketWidget()
{
    if (this->layout() != nullptr) {
        delete this->layout();
    }
    delete socketManager;
}

void SocketWidget::createSetConnectLayout(QGridLayout *parentLayout)
{
    // 24.2.25 :: 현재 사용하면 프로그램 터짐. - 수정필요.
    // ip, port, protocol
    // 172.30.1.43
    QHBoxLayout *boxLayout = new QHBoxLayout();
    QLabel *ipLabel = new QLabel("Address: ", this);
    QLineEdit *ipEdit = new QLineEdit();
    QLabel *portLabel = new QLabel("Port: ", this);
    QLineEdit *portEdit = new QLineEdit();
    
    portLabel->setContentsMargins(10, 0, 0, 0);       //left, top, right, bottom
    ipEdit->setFixedWidth(130);
    portEdit->setFixedWidth(60);
    ipEdit->setMaxLength(calculateMaxCharacters(ipEdit));
    portEdit->setMaxLength(calculateMaxCharacters(portEdit));
    
    boxLayout->addWidget(ipLabel, Qt::AlignLeft);
    boxLayout->addWidget(ipEdit, Qt::AlignLeft);
    boxLayout->addWidget(portLabel, Qt::AlignLeft);
    boxLayout->addWidget(portEdit, Qt::AlignLeft);
    
    parentLayout->addLayout(boxLayout, m_nLayoutRow, 0);
}

void SocketWidget::createConnectionButton(QGridLayout *parentLayout, const QString &labelText, QPushButton *&pushButton)
{
    QPushButton *connectButton = new QPushButton();
    QLabel *label = new QLabel();
    label->setText(labelText);
    parentLayout->addWidget(label);
    parentLayout->addWidget(connectButton);
    // pushButton = connectButton;
    connect(connectButton, SIGNAL(clicked()), this, SLOT(onConnectButtonClicked()));
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

int SocketWidget::calculateMaxCharacters(QLineEdit *lineEdit, bool isNum)
{
    QFontMetrics fm(lineEdit->font());
    int width = lineEdit->width();
    // char c = isNum ? '2' : 'M';
    char c = 'M';
    return width / fm.boundingRect(c).width(); // char 'X' width
}

