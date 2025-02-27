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
    createSendDataLayout(serverLayout);
    createDataLayout(serverLayout);
    createOptionLayout(clientLayout);
    createSendDataLayout(clientLayout);
    createDataLayout(clientLayout);

    setupConnections();
}

SocketWidget::~SocketWidget()
{
    clearConnections();
    if (this->layout() != nullptr) {
        delete this->layout();
    }
    delete socketManager;
}

void SocketWidget::setupConnections()
{
    connect(socketManager, &SocketManager::addClientView, this, &SocketWidget::addClient);
    connect(socketManager, &SocketManager::receiveMessageToLog, this, &SocketWidget::receiveMessage);
    connect(socketTabWidget, &QTabWidget::currentChanged, [=]() {
        int currentTab = socketTabWidget->currentIndex();
        QString tabName = "";
        QString pButton = "";
        switch (currentTab)
        {
        case 0:
            m_nCurrentTab = Communication::Socket::ConnectOption::Server;
            pButton = "Start";
            tabName = "TCP Server";
            break;
        case 1:
            m_nCurrentTab = Communication::Socket::ConnectOption::Client;
            pButton = "Connect";
            tabName = "TCP Client";
            break;
        case 2:
            m_nCurrentTab = Communication::Socket::ConnectOption::Udp;
            pButton = "UDP Server";
            tabName = "UDP";
            break;

        default:
            break;
        }
        optionPushButton->setText(pButton);
        qDebug() << "** Current Tab: " << tabName;
    });
    connect(sendButton, &QPushButton::clicked, this, &SocketWidget::sendMessage);
    connect(logClearButton, &QPushButton::clicked, this, &SocketWidget::logClear);
}

void SocketWidget::clearConnections()
{
    disconnect(socketManager, &SocketManager::addClientView, this, &SocketWidget::addClient);
    disconnect(socketTabWidget, &QTabWidget::currentChanged, this, nullptr);
    disconnect(sendButton, &QPushButton::clicked, this, &SocketWidget::sendMessage);
    disconnect(logClearButton, &QPushButton::clicked, this, &SocketWidget::logClear);
}

void SocketWidget::createTabWidget(QGridLayout *parentLayout)
{
    serverTab = new QWidget();
    serverLayout = new QGridLayout(serverTab);
    clientTab = new QWidget();
    clientLayout = new QGridLayout(clientTab);
    udpTab = new QWidget();
    udpLayout = new QGridLayout(udpTab);

    QLabel *udpLabel = new QLabel(" > To be updated later ... !! ", udpTab);
    udpLayout->addWidget(udpLabel, 0, 0);

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
    ipEdit->setPlaceholderText("0. 0. 0. 0");
    portEdit->setPlaceholderText("00000");
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
    parentLayout->addLayout(boxLayout, autoLayoutRowCount(), 0);
    
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
    if (clientsListView)
    {
        for (ClientInfo &client : clients)
        {
            client.timer->stop();
            delete client.timer;
        }
        clients.clear();
        delete clientsListView;
        delete clientsModel;
    }
    clientsListView = new QListView(this);
    clientsModel = new QStandardItemModel(this);
    clientsListView->setModel(clientsModel);
    clientsListView->setFlow(QListView::LeftToRight);
    clientsListView->setWrapping(false);
    clientsListView->setResizeMode(QListView::Adjust);  // 항목 크기에 맞춰 조정
    clientsListView->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded); // 수평 스크롤바 필요 시 표시
    clientsListView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);  // 수직 스크롤바 비활성화
    clientsListView->setSpacing(3); // 항목 사이 간격 설정
    clientsListView->setMinimumSize(100, 50); // 최소 크기 설정
    clientsListView->setFixedHeight(80);
    clientsListView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    
    // testFunction();

    parentLayout->addWidget(clientsListView, autoLayoutRowCount(), 0, 1, 1, Qt::AlignTop);
}

void SocketWidget::createSendDataLayout(QGridLayout *parentLayout)
{
    sendDataView = new QTextEdit(this);
    sendDataView->setFixedHeight(80);
    sendDataViewLabel = new QLabel("▶️ Send Data :");
    sendDataView->setPlaceholderText("Send to Client(s)...");
    sendDataViewLabel->setContentsMargins(0, 10, 0, 0);   //left, top, right, bottom
    parentLayout->addWidget(sendDataViewLabel, autoLayoutRowCount(), 0);
    parentLayout->addWidget(sendDataView, autoLayoutRowCount(), 0);
    
    sendButton = new QPushButton("Send", this);
    parentLayout->addWidget(sendButton, autoLayoutRowCount(), 0, Qt::AlignRight);
    
}

void SocketWidget::createDataLayout(QGridLayout *parentLayout)
{
    dataViewLabel = new QLabel("▶️ Receive Data :");
    dataViewLabel->setContentsMargins(0, 10, 0, 0);       //left, top, right, bottom
    dataView = new QTextEdit(this);
    dataView->setReadOnly(true);
    dataView->setPlaceholderText("...");
    parentLayout->addWidget(dataViewLabel, autoLayoutRowCount(), 0);
    parentLayout->addWidget(dataView, autoLayoutRowCount(), 0);
    
    logClearButton = new QPushButton("Log Clear", this);
    logClearButton->setFixedSize(100, 25);
    parentLayout->addWidget(logClearButton, autoLayoutRowCount(), 0, Qt::AlignRight);
}

void SocketWidget::addClient(const QString &ip, const QDateTime &connectTime)
{
    if (!clientsModel)
    {
        qDebug() << "Error: clientsModel is null!";
        clientsModel = new QStandardItemModel(this);
        clientsListView->setModel(clientsModel);
    }
    
    ClientInfo client;
    client.ipAddress = ip;
    client.connectTime = connectTime;

    QWidget *clientWidget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(clientWidget);
    QLabel *ipLabel = new QLabel("🖥️ IP : " + ip, clientWidget);
    client.timeLabel = new QLabel("Time : 00:00:00", clientWidget);
    
    ipLabel->setAlignment(Qt::AlignCenter);
    client.timeLabel->setAlignment(Qt::AlignCenter);

    layout->addWidget(ipLabel);
    layout->addWidget(client.timeLabel);
    layout->setContentsMargins(5, 5, 5, 5);

    // add a item to listview.
    QStandardItem *item = new QStandardItem("\u200B"); // Zero Width Space (U+200B)
    item->setSizeHint(QSize(150, 50)); // 원하는 크기로 고정
    clientsModel->appendRow(item);
    QModelIndex index = clientsModel->index(clientsModel->rowCount() - 1, 0);
    clientsListView->setIndexWidget(index, clientWidget);
    qDebug() << "Added client IP:" << ip << "at row:" << index.row();

    // set timer
    client.timer = new QTimer(this);
    connect(client.timer, &QTimer::timeout, this, &SocketWidget::updateElapsedTime);
    client.timer->start(1000);

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
    QFrame *frame = new QFrame(this);
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
        qDebug() << "start button to start server : " << port;
    }
}

void SocketWidget::setupClient(const QString &ip, int port)
{
   if (socketManager->startAsClient(ip, port)) {
       qDebug() << "Connected to server at " << ip << ":" << port;
   }
}

void SocketWidget::sendMessage()
{
    QString message = sendDataView->toPlainText().trimmed();
    
    if (!message.isEmpty())
    {
        if (socketManager->send(message))
        {
            qDebug() << "Sent: " << message;
            sendDataView->clear();  //remove text after send.
        }
        else
        {
            qDebug() << "Failed to send: " << message;
        }
    }
    else
    {
        qDebug() << "No message to send!";
    }
}

void SocketWidget::receiveMessage(const QString &message)
{
    if (message != "")
    {
        qDebug() << "Received: " << message;
        dataView->append(message);
        dataView->ensureCursorVisible();
    }
    else
    {
        qDebug() << "message is empty !!";
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

int SocketWidget::autoLayoutRowCount()
{
    // 한 기능 내에 top-down 으로 순차적이므로 사용순서 주의.
    int currentRow = m_nLayoutRow;
    m_nLayoutRow++;
    return currentRow;
}

void SocketWidget::logClear()
{
    dataView->clear();
}

