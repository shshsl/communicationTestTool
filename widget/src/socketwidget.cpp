#include "widget/include/socketwidget.h"

SocketWidget::SocketWidget(QWidget *parent) 
    : QWidget(parent), socketManager(new SocketManager(this)),
    m_nLayoutRow(0), m_nLayoutColumn(0)
{
    // UI 생성
    QGridLayout *layout = new QGridLayout(this);
    setLayout(layout);

    createTabWidget(layout);
    createOptionLayout(serverLayout);
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

void SocketWidget::createTabWidget(QGridLayout *parentLayout)
{
    socketTabWidget = new QTabWidget(this);
    serverTab = new QWidget();
    serverLayout = new QGridLayout(serverTab);
    clientTab = new QWidget();
    clientLayout = new QGridLayout(clientTab);
    udpTab = new QWidget();
    udpLayout = new QGridLayout(udpTab);

    socketTabWidget->addTab(serverTab, "TCP Server");
    socketTabWidget->addTab(clientTab, "TCP Client");
    socketTabWidget->addTab(udpTab, "UDP");

    parentLayout->addWidget(socketTabWidget);
}

void SocketWidget::createOptionLayout(QGridLayout *parentLayout)
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
    ipEdit->setFixedWidth(110);
    portEdit->setFixedWidth(55);
    ipEdit->setMaxLength(15);   // ip: 0.0.0.0 ~ 255.255.255.255
    portEdit->setMaxLength(5);   // port: 1025 ~ 65535
    
    boxLayout->addWidget(ipLabel);
    boxLayout->addWidget(ipEdit);
    boxLayout->addWidget(portLabel);
    boxLayout->addWidget(portEdit);
    
    boxLayout->addStretch(); // or>> boxLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
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

// check for edit size.
int SocketWidget::resizeWidthForEdit(QLineEdit *lineEdit, Communication::Socket::ConnectOption option)
{
    QString optionStr = "";
    switch (option)
    {
    case Communication::Socket::ConnectOption::IP_ADDRESS:
        optionStr = "255.255.255.255";
        break;
    case Communication::Socket::ConnectOption::PORT:
        optionStr = "65535";
        break;
    
    default:
        optionStr = "00000 00000 00000 00000";
        break;
    }
    
    QFontMetrics fm(lineEdit->font());
    int textWidth = fm.width(optionStr);

    QStyleOptionFrame styleOption;
    styleOption.initFrom(lineEdit);
    int padding = lineEdit->style()->pixelMetric(QStyle::PM_DefaultFrameWidth, &styleOption, lineEdit) * 2; // left, right
    int extraSpace = 20; // cusor or extra

    qDebug() << optionStr << " : " << textWidth + padding + extraSpace;
    return textWidth + padding + extraSpace;
}

