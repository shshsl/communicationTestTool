#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QListView>
#include <QGroupBox>
#include <QDebug>
#include <QSerialPortInfo>
#include <QFrame>

#include "include/mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), serialPort(new QSerialPort(this)), logModel(new QStandardItemModel(this))
{
    // UI 구성        
    auto *mainWidget = new QWidget(this);
    auto *mainLayout = new QGridLayout(mainWidget);

    // QFrame 생성 및 설정 - layout 위치 확인용 test.
    QFrame *frame = new QFrame;
    frame->setFrameShape(QFrame::Box);  // 사각형 테두리
    frame->setFrameShadow(QFrame::Plain);  // 단순한 테두리 스타일
    frame->setLineWidth(2);  // 테두리 두께 설정
    mainLayout->addWidget(frame);

    // 로그를 표시할 QListView와 모델 설정
    auto *listView = new QListView(this);
    listView->setModel(logModel);
    mainLayout->addWidget(listView, 0, 0);

    // [ Options ]
    auto *optionGroupBox = new QGroupBox();
    auto *optionGroupLayout = new QVBoxLayout();
    mainLayout->addWidget(optionGroupBox, 0, 1);

    // 포트 선택 ComboBox 생성 및 초기화
    QHBoxLayout *portBoxLayout = new QHBoxLayout();
    portLabel = new QLabel(this);
    portLabel->setText("COM port : ");
    portSelector = new QComboBox(this);
    populateAvailablePorts();

    portBoxLayout->addWidget(portLabel);
    portBoxLayout->addWidget(portSelector);
    optionGroupLayout->addLayout(portBoxLayout);
    optionGroupBox->setLayout(optionGroupLayout);

    // Baudrate 선택 ComboBox 생성 및 초기화
    QHBoxLayout *baudBoxLayout = new QHBoxLayout();
    baudLabel = new QLabel(this);
    baudLabel->setText("Baud Rate : ");
    baudrateSelector = new QComboBox(this);
    baudrateSelector->addItems({"2400", "4800", "9600", "14400", "19200", "38400", "57600", "115200"});  // 일반적으로 사용되는 Baudrate 값 추가
    baudrateSelector->setCurrentText("115200");  // 기본값 설정

    baudBoxLayout->addWidget(baudLabel);
    baudBoxLayout->addWidget(baudrateSelector);
    optionGroupLayout->addLayout(baudBoxLayout);
    optionGroupBox->setLayout(optionGroupLayout);

    // DataBits 선택 ComboBox 생성 및 초기화
    QHBoxLayout *dataBitsBoxLayout = new QHBoxLayout();
    dataBitsLabel = new QLabel(this);
    dataBitsLabel->setText("Data Bits : ");
    dataBitsSelector = new QComboBox(this);
    dataBitsSelector->addItems({"5", "6", "7", "8"});  // Data5, Data6, Data7, Data8
    dataBitsSelector->setCurrentText("8");

    dataBitsBoxLayout ->addWidget(dataBitsLabel);
    dataBitsBoxLayout ->addWidget(dataBitsSelector);
    optionGroupLayout->addLayout(dataBitsBoxLayout);
    optionGroupBox->setLayout(optionGroupLayout);

    // Parity 선택 ComboBox 생성 및 초기화
    QHBoxLayout *parityBoxLayout = new QHBoxLayout();
    parityLabel = new QLabel(this);
    parityLabel->setText("Data Bits : ");
    paritySelector = new QComboBox(this);
    paritySelector->addItems({"No Parity", "Even Parity", "Odd Parity", "Space Parity", "Mark Parity"});
    paritySelector->setCurrentText("No Parity");

    parityBoxLayout ->addWidget(parityLabel);
    parityBoxLayout ->addWidget(paritySelector);
    optionGroupLayout->addLayout(parityBoxLayout);
    optionGroupBox->setLayout(optionGroupLayout);

    // StopBits 선택 ComboBox 생성 및 초기화
    QHBoxLayout *stopBitsBoxLayout = new QHBoxLayout();
    stopBitsLabel = new QLabel(this);
    stopBitsLabel->setText("Stop Bits : ");
    stopBitsSelector = new QComboBox(this);
    stopBitsSelector->addItems({"1", "1.5", "2"});  // OneStop, OneAndHalfStop, TwoStop
    stopBitsSelector->setCurrentText("1");

    stopBitsBoxLayout->addWidget(stopBitsLabel);
    stopBitsBoxLayout->addWidget(stopBitsSelector);
    optionGroupLayout->addLayout(stopBitsBoxLayout);
    optionGroupBox->setLayout(optionGroupLayout);

    // FlowControl 선택 ComboBox 생성 및 초기화
    QHBoxLayout *flowBoxLayout = new QHBoxLayout();
    flowLabel = new QLabel(this);
    flowLabel->setText("Flow Control : ");
    flowControlSelector = new QComboBox(this);
    flowControlSelector->addItems({"No Flow Control", "Hardware Control", "Software Control"});
    flowControlSelector->setCurrentText("No Flow Control");

    flowBoxLayout->addWidget(flowLabel);
    flowBoxLayout->addWidget(flowControlSelector);
    optionGroupLayout->addLayout(flowBoxLayout);
    optionGroupBox->setLayout(optionGroupLayout);

    // [ Button ]
    // Open 및 Close 버튼 생성
    openButton = new QPushButton("Open", this);
    closeButton = new QPushButton("Close", this);

    openButton->setFixedSize(400, 50);
    closeButton->setFixedSize(400, 50);

    // 버튼을 포함한 하단 레이아웃 생성
    auto *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(openButton);
    buttonLayout->addWidget(closeButton);
    mainLayout->addLayout(buttonLayout, 1, 0);

    //clear button
    clearBtn = new QPushButton("Clear", this);
    clearBtn->setFixedSize(100, 25);
    mainLayout->addWidget(clearBtn, 1, 1);

    setCentralWidget(mainWidget);

    // 초기 상태: Close 버튼 비활성화
    closeButton->setEnabled(false);

    // 시그널-슬롯 연결
    connect(openButton, &QPushButton::clicked, this, &MainWindow::openSerialPort);
    connect(closeButton, &QPushButton::clicked, this, &MainWindow::closeSerialPort);
    connect(clearBtn, &QPushButton::clicked, this, &MainWindow::logClear);
}

MainWindow::~MainWindow()
{
    if (serialPort->isOpen()) {
        serialPort->close();
    }
}

void MainWindow::populateAvailablePorts() {
    portSelector->clear();  // 기존 항목 제거

    const auto ports = QSerialPortInfo::availablePorts();

    // 연결된 포트를 검색하여 ComboBox에 추가
    for (const QSerialPortInfo &port : ports)
    {
        portSelector->addItem(port.portName());
        qDebug() << "Found port:" << port.portName();
    }

    // 연결된 포트가 없을 경우 메시지 표시 및 Open 버튼 비활성화
    if (ports.isEmpty())
    {
        portSelector->addItem("No Ports Available");
        logModel->appendRow(new QStandardItem("** No Ports Available **"));
        qDebug() << "No Ports Available";
    }
    else
    {
        logModel->appendRow(new QStandardItem("+++++  ++++++++++++++++++++++++++"));
        logModel->appendRow(new QStandardItem("▶️ There is currently an available COM port. "));
        logModel->appendRow(new QStandardItem("+++++  ++++++++++++++++++++++++++"));
        qDebug() << "Ready To Do Something. ";
    }
}

void MainWindow::readSerialData()
{
    while (serialPort->canReadLine())
    {  // 줄 단위로 데이터 읽기
        QByteArray data = serialPort->readLine();
        QString logEntry = QString::fromUtf8(data).trimmed();

        // QListView에 새로운 로그 추가
        logModel->appendRow(new QStandardItem(logEntry));

        // 로그가 많아지면 자동 스크롤
        auto *listView = qobject_cast<QListView *>(centralWidget()->layout()->itemAt(0)->widget());
        if (listView)
        {
            listView->scrollToBottom();
        }

        qDebug() << "Received:" << logEntry;  // 디버그 출력
    }
}

void MainWindow::openSerialPort()
{
    QString selectedPortName = portSelector->currentText();  // 사용자가 선택한 포트 이름 가져오기
    QString selectedBaudRate = baudrateSelector->currentText();  // 사용자가 선택한 Baudrate 가져오기
    QString selectedDataBits = dataBitsSelector->currentText();
    QString selectedParity = paritySelector->currentText();
    QString selectedStopBits = stopBitsSelector->currentText();
    QString selectedFlowControl = flowControlSelector->currentText();

    serialPort->setPortName(selectedPortName);  // 선택한 포트를 설정
    serialPort->setBaudRate(selectedBaudRate.toInt());  // 선택한 Baudrate 설정

    // Data Bits 설정
    if (selectedDataBits == "5") serialPort->setDataBits(QSerialPort::Data5);
    else if (selectedDataBits == "6") serialPort->setDataBits(QSerialPort::Data6);
    else if (selectedDataBits == "7") serialPort->setDataBits(QSerialPort::Data7);
    else serialPort->setDataBits(QSerialPort::Data8);

    // Parity 설정
    if (selectedParity == "No Parity") serialPort->setParity(QSerialPort::NoParity);
    else if (selectedParity == "Even Parity") serialPort->setParity(QSerialPort::EvenParity);
    else if (selectedParity == "Odd Parity") serialPort->setParity(QSerialPort::OddParity);
    else if (selectedParity == "Space Parity") serialPort->setParity(QSerialPort::SpaceParity);
    else serialPort->setParity(QSerialPort::MarkParity);

    // Stop Bits 설정
    if (selectedStopBits == "1") serialPort->setStopBits(QSerialPort::OneStop);
    else if (selectedStopBits == "1.5") serialPort->setStopBits(QSerialPort::OneAndHalfStop);
    else serialPort->setStopBits(QSerialPort::TwoStop);

    // Flow Control 설정
    if (selectedFlowControl == "No Flow Control") serialPort->setFlowControl(QSerialPort::NoFlowControl);
    else if (selectedFlowControl == "Hardware Control") serialPort->setFlowControl(QSerialPort::HardwareControl);
    else serialPort->setFlowControl(QSerialPort::SoftwareControl);

    // port 가능 여부 판단. ---
    if (serialPort->open(QIODevice::ReadOnly))
    {
        connect(serialPort, &QSerialPort::readyRead, this, &MainWindow::readSerialData);

        logModel->appendRow(new QStandardItem("> Opened port: [ " + selectedPortName + " ] at [ " + selectedBaudRate + " ] baud"));
        qDebug() << "Opened port:" << selectedPortName << "at" << selectedBaudRate << "baud";

        optionStateChanged(SERIAL_PORT_STATE::SERIAL_PORT_CLOSE);   //toggle
    }
    else
    {
        serialPort->close();
        logModel->appendRow(new QStandardItem("** !.!. Failed to open port: [ " + selectedPortName + " ]"));
        qDebug() << "Failed to open port:" << serialPort->errorString();

        optionStateChanged(SERIAL_PORT_STATE::SERIAL_PORT_OPEN);    //toggle
    }
}

void MainWindow::closeSerialPort()
{
    if (serialPort->isOpen())
    {
        disconnect(serialPort, &QSerialPort::readyRead, this, &MainWindow::readSerialData);
        serialPort->close();
        logModel->appendRow(new QStandardItem("+++++=++++++++++++++++++++++++++"));
        logModel->appendRow(new QStandardItem("■ Closed port."));
        logModel->appendRow(new QStandardItem("+++++=++++++++++++++++++++++++++"));
        qDebug() << "Closed port.";

        optionStateChanged(SERIAL_PORT_STATE::SERIAL_PORT_OPEN);

        populateAvailablePorts();  // 사용 가능한 포트를 다시 검색
    }
//    optionStateChanged(m_nClose);
}

void MainWindow::logClear()
{
    logModel->clear();
    optionStateChanged(SERIAL_PORT_STATE::SERIAL_PORT_NONE);
}

void MainWindow::optionStateChanged(int oState)
{
    if (oState == SERIAL_PORT_STATE::SERIAL_PORT_CLOSE)
    {
        openButton->setEnabled(false);   // Open 버튼 활성화
        closeButton->setEnabled(true);  // Close 버튼 비활성화
        portSelector->setEnabled(false);  // 포트 선택 활성화
        baudrateSelector->setEnabled(false);  // Baudrate 선택 활성화
        dataBitsSelector->setEnabled(false);
        paritySelector->setEnabled(false);
        stopBitsSelector->setEnabled(false);
        flowControlSelector->setEnabled(false);
    }
    else
    {
        openButton->setEnabled(true);   // Open 버튼 활성화
        closeButton->setEnabled(false);  // Close 버튼 비활성화
        portSelector->setEnabled(true);  // 포트 선택 활성화
        baudrateSelector->setEnabled(true);  // Baudrate 선택 활성화
        dataBitsSelector->setEnabled(true);
        paritySelector->setEnabled(true);
        stopBitsSelector->setEnabled(true);
        flowControlSelector->setEnabled(true);
    }
}
