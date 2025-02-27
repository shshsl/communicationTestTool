#include "widget/include/socketwidget.h"

SocketWidget::SocketWidget(QWidget *parent) 
    : QWidget(parent), socketManager(new SocketManager(this))
    , m_nLayoutRow(0), m_nLayoutColumn(0), clientsLayout(nullptr), socketTabWidget(new QTabWidget(this))
    , clientsListView(nullptr), clientsModel(nullptr)
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
        QString pButton = "";
        switch (currentTab)
        {
        case 0:
            m_nCurrentTab = Communication::Socket::ConnectOption::Server;
            pButton = "Start";
            break;
        case 1:
            m_nCurrentTab = Communication::Socket::ConnectOption::Client;
            pButton = "Connect";
            break;
        case 2:
            m_nCurrentTab = Communication::Socket::ConnectOption::Udp;
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
    boxLayout->addStretch();
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
    // 기존 clientsListView와 model 정리
    if (clientsListView) {
        for (ClientInfo &client : clients) {
            client.timer->stop();
            delete client.timer;
        }
        clients.clear();
        delete clientsListView;
        delete clientsModel;
    }

    // QListView와 모델 생성
    clientsListView = new QListView(this);
    clientsModel = new QStandardItemModel(this);
    clientsListView->setModel(clientsModel);

    // 리스트 뷰 설정
    clientsListView->setFlow(QListView::LeftToRight);   // 좌->우 흐름 설정
    clientsListView->setWrapping(false);                // 줄 바꿈 비활성화
    clientsListView->setResizeMode(QListView::Adjust);  // 항목 크기에 맞춰 조정
    clientsListView->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded); // 수평 스크롤바 필요 시 표시
    clientsListView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);  // 수직 스크롤바 비활성화
    clientsListView->setSpacing(3); // 항목 사이 간격 설정
    clientsListView->setMinimumSize(100, 50); // 최소 크기 설정
    clientsListView->setFixedHeight(80);
    clientsListView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    
    // testFunction();

    parentLayout->addWidget(clientsListView, m_nLayoutRow + 1, 0, 1, 1, Qt::AlignTop);
}

void SocketWidget::addClient(const QString &ip, const QDateTime &connectTime)
{
    qDebug() << "widget - addClient !! " << ip << " , " << connectTime;
    
    // clientsModel이 null일 경우 초기화
    if (!clientsModel) {
        qDebug() << "Error: clientsModel is null!";
        clientsModel = new QStandardItemModel(this);
        clientsListView->setModel(clientsModel);
    }
    
    // ClientInfo 객체 생성
    ClientInfo client;
    client.ipAddress = ip;
    client.connectTime = connectTime;

    // 클라이언트 위젯 생성
    QWidget *clientWidget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(clientWidget);
    QLabel *ipLabel = new QLabel("🖥️ IP : " + ip, clientWidget);
    client.timeLabel = new QLabel("Time : 00:00:00", clientWidget);
    
    ipLabel->setAlignment(Qt::AlignCenter);
    client.timeLabel->setAlignment(Qt::AlignCenter);

    layout->addWidget(ipLabel);
    layout->addWidget(client.timeLabel);
    layout->setContentsMargins(5, 5, 5, 5);

    QStandardItem *item = new QStandardItem("\u200B"); // Zero Width Space (U+200B)
    item->setSizeHint(QSize(150, 50)); // 원하는 크기로 고정
    clientsModel->appendRow(item);
    QModelIndex index = clientsModel->index(clientsModel->rowCount() - 1, 0);
    clientsListView->setIndexWidget(index, clientWidget);
    qDebug() << "Added client IP:" << ip << "at row:" << index.row();

    // 타이머 설정
    client.timer = new QTimer(this);
    connect(client.timer, &QTimer::timeout, this, &SocketWidget::updateElapsedTime);
    client.timer->start(1000);

    // 클라이언트 리스트에 추가
    clients.append(client);
    updateElapsedTime();
}

void SocketWidget::updateElapsedTime()
{
    for (ClientInfo &client : clients)
    {
        QDateTime now = QDateTime::currentDateTime();
        qint64 seconds = client.connectTime.secsTo(now);
        QTime time(0, 0, 0);
        time = time.addSecs(seconds);
        client.timeLabel->setText("Time : " + time.toString("hh:mm:ss"));
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
    case Communication::Socket::ConnectOption::IpAddress:
        optionStr = "255.255.255.255";
        break;
    case Communication::Socket::ConnectOption::Port:
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
    // createOptionLayout : 기본값 설정 
    ipEdit->setText("172.30.1.43");  // 테스트용 기본 IP
    portEdit->setText("8080");       // 테스트용 기본 포트
    
    // 예시 클라이언트 추가 (테스트용)
    // addClient("192.168.1.1", QDateTime::currentDateTime());
    // addClient("172.30.1.43", QDateTime::currentDateTime().addSecs(-3600)); // 1시간 전
    
    // // createClientsView : 테스트 list 항목 추가
    // QStandardItem *testItem = new QStandardItem("Test Client");
    // clientsModel->appendRow(testItem);
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

    if (m_nCurrentTab == Communication::Socket::ConnectOption::Server)
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
    else if (m_nCurrentTab == Communication::Socket::ConnectOption::Client)
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
