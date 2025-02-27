#include "widget/include/socketwidget.h"

SocketWidget::SocketWidget(QWidget *parent) 
    : QWidget(parent), socketManager(new SocketManager(this)),
    m_nLayoutRow(0), m_nLayoutColumn(0), clientsLayout(nullptr), socketTabWidget(new QTabWidget(this))
{
    // UI 생성
    QGridLayout *layout = new QGridLayout(this);
    setLayout(layout);
    layout->addWidget(socketTabWidget);

    createTabWidget(layout);
    createOptionLayout(serverLayout);
    createClientsView(serverLayout);
    createOptionLayout(clientLayout);

    connect(socketManager, &SocketManager::addClientView, this, &SocketWidget::addClient);
    QObject::connect(socketTabWidget, &QTabWidget::currentChanged, [=]() {
        int currentTab = socketTabWidget->currentIndex();
        // label->setText("current tab: " + QString::number(currentTab));
        m_nCurrentTab = currentTab;
        QString pButton = "";
        switch (currentTab)
        {
        case 0:
            pButton = "Start";
            break;
        case 1:
            pButton = "Connect";
            break;
        case 2:
            pButton = "UDP Server";
            break;

        default:
            break;
        }
        optionPushButton->setText(pButton);
        qDebug() << "currentTab: " << currentTab;
    });
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
    // ip, port, protocol
    // 172.30.1.43
    QHBoxLayout *boxLayout = new QHBoxLayout();
    QLabel *ipLabel = new QLabel("Address: ", this);
    ipEdit = new QLineEdit();
    QLabel *portLabel = new QLabel("Port: ", this);
    portEdit = new QLineEdit();
    
    optionPushButton = new QPushButton("Start", this);
    
    testFunction();
    
    portLabel->setContentsMargins(10, 0, 0, 0);       //left, top, right, bottom
    ipEdit->setFixedWidth(110);
    portEdit->setFixedWidth(55);
    ipEdit->setMaxLength(15);   // ip: 0.0.0.0 ~ 255.255.255.255
    portEdit->setMaxLength(5);   // port: 1025 ~ 65535
    
    boxLayout->addWidget(ipLabel);
    boxLayout->addWidget(ipEdit);
    boxLayout->addWidget(portLabel);
    boxLayout->addWidget(portEdit);
    boxLayout->addWidget(optionPushButton);
    
    boxLayout->addStretch(); // or>> boxLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
    parentLayout->addLayout(boxLayout, m_nLayoutRow, 0);
    
    connect(ipEdit, &QLineEdit::textChanged, this, [](const QString &text) {
        qDebug() << "IP changed to: " << text;
    });
    connect(portEdit, &QLineEdit::textChanged, this, [](const QString &text) {
        qDebug() << "Port changed to: " << text;
    });
    connect(optionPushButton, &QPushButton::clicked, this, &SocketWidget::onOptionButtonClicked);
}

void SocketWidget::createClientsView(QGridLayout *parentLayout)
{
    // 기존 clientsLayout 정리
    if (clientsLayout) {
        QLayoutItem *item;
        while ((item = clientsLayout->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
        delete clientsLayout;

        for (ClientInfo &client : clients) {
            client.timer->stop();
            delete client.timer;
        }
        clients.clear();
    }

    // 새로운 가로 레이아웃 생성
    clientsLayout = new QHBoxLayout();
    clientsLayout->setSpacing(10); // 클라이언트 간 간격
    clientsLayout->addStretch();   // 오른쪽 여백 추가

    // testFunction();  //for test - (25.2.26)
    
    // QGridLayout에 clientsLayout 추가
    parentLayout->addLayout(clientsLayout, m_nLayoutRow + 1, 0); // 탭 아래에 배치 (행 조정 가능)
}

void SocketWidget::addClient(const QString &ip, const QDateTime &connectTime)
{
    // 클라이언트 정보 생성
    ClientInfo client;
    client.ipAddress = ip;
    client.connectTime = connectTime;

    createFrameBox();

    // 클라이언트 위젯 생성 및 레이아웃 설정
    QWidget *clientsViewWidget = new QWidget(frame); // frame을 부모로 설정
    QVBoxLayout *layout = new QVBoxLayout(clientsViewWidget);
    QLabel *ipLabel = new QLabel("🖥️ IP : " + ip, clientsViewWidget);
    client.timeLabel = new QLabel("Time : 00:00:00", clientsViewWidget);

    layout->addWidget(ipLabel);
    layout->addWidget(client.timeLabel);
    layout->setContentsMargins(5, 5, 5, 5);

    // QFrame에 clientsWidget 추가
    QVBoxLayout *frameLayout = new QVBoxLayout(frame);
    frameLayout->addWidget(clientsViewWidget);
    frameLayout->setContentsMargins(0, 0, 0, 0); // 프레임 내부 여백 제거 (필요 시 조정)

    // 타이머 설정
    client.timer = new QTimer(this);
    connect(client.timer, &QTimer::timeout, this, &SocketWidget::updateElapsedTime);
    client.timer->start(1000); // 1초 간격

    // 클라이언트 목록에 추가
    clients.append(client);
    clientsLayout->insertWidget(clientsLayout->count() - 1, frame); // frame을 추가

    // 초기 시간 업데이트
    updateElapsedTime();
}

void SocketWidget::updateElapsedTime()
{
    for (ClientInfo &client : clients) {
        qint64 seconds = client.connectTime.secsTo(QDateTime::currentDateTime());
        QTime elapsedTime = QTime(0, 0).addSecs(seconds);
        client.timeLabel->setText("Time: " + elapsedTime.toString("hh:mm:ss"));
    }
}

void SocketWidget::createFrameBox()
{
    frame = new QFrame(this);
    frame->setFrameShape(QFrame::Box);
    frame->setFrameShadow(QFrame::Plain);
    frame->setLineWidth(2);
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

void SocketWidget::testFunction()
{
    // 기본값 설정
    ipEdit->setText("172.30.1.43");  // 테스트용 기본 IP
    portEdit->setText("8080");       // 테스트용 기본 포트
    
    // 예시 클라이언트 추가 (테스트용)
    // addClient("192.168.1.1", QDateTime::currentDateTime());
    // addClient("172.30.1.43", QDateTime::currentDateTime().addSecs(-3600)); // 1시간 전
}

void SocketWidget::setupServer(int port)
{
    if (socketManager->startAsServer(port)) {
        qDebug() << "Server started on port " << port;
    }
}

void SocketWidget::setupClient(const QString &ip, int port)
{
   if (socketManager->startAsClient(ip, port)) {
       qDebug() << "Connected to server at " << ip << ":" << port;
   }
}

void SocketWidget::sendMessage(const QString &message)
{
    if (socketManager->send(message))
    {
        qDebug() << "Sent: " << message;
    }
}

void SocketWidget::receiveMessage()
{
    QString msg = socketManager->receive();
    if (msg != "")
    {
        qDebug() << "Received: " << msg;
    }
    else
    {
        qDebug() << "msg is empty !!";
    }
}

void SocketWidget::onOptionButtonClicked()
{
    QString ip = ipEdit->text().trimmed();
    QString portText = portEdit->text().trimmed();
    bool ok;
    int port = portText.toInt(&ok);

    qDebug() << "IP: " << ip << " | Port: " << portText;

    if (m_nCurrentTab == 0) // 서버
    {
        if (ok && port >= 1025 && port <= 65535)
        {
            setupServer(port);
        }
        else
        {
            qDebug() << "[Server] Invalid port! Enter a number between 1025 and 65535.";
        }
    }
    else if (m_nCurrentTab == 1) // 클라이언트
    {
        QHostAddress address(ip);
        if (!ip.isEmpty() && !address.isNull() && ok && port >= 1025 && port <= 65535)
        {
            setupClient(ip, port);
        }
        else
        {
            qDebug() << "[Client] Invalid input! Ensure IP is valid and port is between 1025 and 65535.";
        }
    }
    else
    {
        qDebug() << "UDP mode not implemented yet.";
    }
}


// ipv4 유효성 검사 Test 용.
// #include <QRegExp>

// bool isValidIPv4(const QString &ip)
// {
//     QRegExp ipPattern("^(\\d{1,3})\\.(\\d{1,3})\\.(\\d{1,3})\\.(\\d{1,3})$");
//     if (!ipPattern.exactMatch(ip))
//         return false;

//     QStringList parts = ip.split(".");
//     for (const QString &part : parts)
//     {
//         bool ok;
//         int num = part.toInt(&ok);
//         if (!ok || num < 0 || num > 255)
//             return false;
//     }
//     return true;
// }
