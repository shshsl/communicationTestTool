// void UartWidget::readSerialData()
// {
//     while (serialPort->canReadLine())
//     {
//         // 줄 단위로 데이터 읽기
//         QByteArray data = serialPort->readLine();
//         QString logEntry = QString::fromUtf8(data).trimmed();

//         // QListView에 새로운 로그 추가
//         logModel->appendRow(new QStandardItem(logEntry));

//         // 로그가 많아지면 자동 스크롤
//         auto *mainLayout = qobject_cast<QGridLayout *>(layout());
//         if (mainLayout)
//         {
//             // 0행 0열에 위치한 QListView 가져오기
//             auto *item = mainLayout->itemAtPosition(0, 0);
//             if (item)
//             {
//                 auto *listView = qobject_cast<QListView *>(item->widget());
//                 if (listView)
//                 {
//                     listView->scrollToBottom();
//                 }
//             }
//         }

//         qDebug() << "Received:" << logEntry; // 디버그 출력
//     }
// }


// void UartWidget::openSerialPort()
// {
//     QString selectedPortName = portSelector->currentText();  // 사용자가 선택한 포트 이름 가져오기
//     QString selectedBaudRate = baudrateSelector->currentText();  // 사용자가 선택한 Baudrate 가져오기
//     QString selectedDataBits = dataBitsSelector->currentText();
//     QString selectedParity = paritySelector->currentText();
//     QString selectedStopBits = stopBitsSelector->currentText();
//     QString selectedFlowControl = flowControlSelector->currentText();

//     serialPort->setPortName(selectedPortName);  // 선택한 포트를 설정
//     serialPort->setBaudRate(selectedBaudRate.toInt());  // 선택한 Baudrate 설정

//     // Data Bits 설정
//     if (selectedDataBits == "5") serialPort->setDataBits(QSerialPort::Data5);
//     else if (selectedDataBits == "6") serialPort->setDataBits(QSerialPort::Data6);
//     else if (selectedDataBits == "7") serialPort->setDataBits(QSerialPort::Data7);
//     else serialPort->setDataBits(QSerialPort::Data8);

//     // Parity 설정
//     if (selectedParity == "No Parity") serialPort->setParity(QSerialPort::NoParity);
//     else if (selectedParity == "Even Parity") serialPort->setParity(QSerialPort::EvenParity);
//     else if (selectedParity == "Odd Parity") serialPort->setParity(QSerialPort::OddParity);
//     else if (selectedParity == "Space Parity") serialPort->setParity(QSerialPort::SpaceParity);
//     else serialPort->setParity(QSerialPort::MarkParity);

//     // Stop Bits 설정
//     if (selectedStopBits == "1") serialPort->setStopBits(QSerialPort::OneStop);
//     else if (selectedStopBits == "1.5") serialPort->setStopBits(QSerialPort::OneAndHalfStop);
//     else serialPort->setStopBits(QSerialPort::TwoStop);

//     // Flow Control 설정
//     if (selectedFlowControl == "No Flow Control") serialPort->setFlowControl(QSerialPort::NoFlowControl);
//     else if (selectedFlowControl == "Hardware Control") serialPort->setFlowControl(QSerialPort::HardwareControl);
//     else serialPort->setFlowControl(QSerialPort::SoftwareControl);

//     // test...
//     // 비동기 작업 실행

//     m_selectedPort = selectedPortName;
//     m_selectedBaudRate = selectedBaudRate;

//     showProgressDialog();


// //    // port 가능 여부 판단. ---
// //    if (serialPort->open(QIODevice::ReadOnly))
// //    {
// //        connect(serialPort, &QSerialPort::readyRead, this, &MainWindow::readSerialData);

// //        logModel->appendRow(new QStandardItem("> Opened port: [ " + selectedPortName + " ] at [ " + selectedBaudRate + " ] baud"));
// //        qDebug() << "Opened port:" << selectedPortName << "at" << selectedBaudRate << "baud";

// //        optionStateChanged(SERIAL_PORT_STATE::SERIAL_PORT_CLOSE);   //toggle
// //    }
// //    else
// //    {
// //        serialPort->close();
// //        logModel->appendRow(new QStandardItem("** !.!. Failed to open port: [ " + selectedPortName + " ]"));
// //        qDebug() << "Failed to open port:" << serialPort->errorString();

// //        optionStateChanged(SERIAL_PORT_STATE::SERIAL_PORT_OPEN);    //toggle
// //    }
// }

// void UartWidget::closeSerialPort()
// {
//     if (serialPort->isOpen())
//     {
//         disconnect(serialPort, &QSerialPort::readyRead, this, &UartWidget::readSerialData);
//         serialPort->close();
//         logModel->appendRow(new QStandardItem("+++++=++++++++++++++++++++++++++"));
//         logModel->appendRow(new QStandardItem("■ Closed port."));
//         logModel->appendRow(new QStandardItem("+++++=++++++++++++++++++++++++++"));
//         qDebug() << "Closed port.";

//         optionStateChanged(SERIAL_PORT_STATE::SERIAL_PORT_OPEN);

//         populateAvailablePorts();  // 사용 가능한 포트를 다시 검색
//     }
// //    optionStateChanged(m_nClose);
// }

// //// 비동기 UI 업데이트.
// void UartWidget::showProgressDialog() {
//     // QProgressDialog 생성
//     QProgressDialog* progress = new QProgressDialog("포트를 여는 중입니다...", "취소", 0, 0, this);
//     progress->setWindowModality(Qt::WindowModal);
//     progress->setCancelButton(nullptr); // 취소 버튼 숨김
//     progress->show();

//     // Worker 스레드 생성
//     QThread* workThread = new QThread(this);
//     serialPort->moveToThread(workThread);

//     connect(workThread, &QThread::started, [=]() {
//         bool success = serialPort->open(QIODevice::ReadOnly);

//         emit threadFinished(success); // 결과 전달
//         workThread->quit();
//     });

//     connect(workThread, &QThread::finished, this, [=]() {
//         progress->close();

//         if (serialPort->isOpen())
//         {
//             connect(serialPort, &QSerialPort::readyRead, this, &UartWidget::readSerialData);

//             logModel->appendRow(new QStandardItem("> Opened port: [ " + m_selectedPort + " ] at [ " + m_selectedBaudRate + " ] baud"));
//             qDebug() << "Opened port:" << m_selectedPort << "at" << m_selectedBaudRate << "baud";

//             optionStateChanged(SERIAL_PORT_STATE::SERIAL_PORT_CLOSE);   // toggle
//         }
//         else
//         {
//             QMessageBox::critical(this, "실패", "포트를 열 수 없습니다.");

//             logModel->appendRow(new QStandardItem("** !.!. Failed to open port: [ " + m_selectedPort + " ]"));
//             qDebug() << "Failed to open port:" << serialPort->errorString();

//             optionStateChanged(SERIAL_PORT_STATE::SERIAL_PORT_OPEN);    // toggle
//         }

//         workThread->deleteLater();
// //        serialPort->deleteLater();    // serialport는 closebutton 때만 제거.
//         progress->deleteLater();
//     });

//     workThread->start();
// }




/////////////////////////===================================================================

#include "widget/include/uartwidget.h"

UartWidget::UartWidget(QWidget *parent)
   : QWidget(parent), uartManager(new UartManager(this)),
   logModel(new QStandardItemModel(this))
{
   // UI 구성
   auto *gLayout = new QGridLayout(this);
   setLayout(gLayout);
   
   // // QFrame 생성 및 설정 - layout 위치 확인용 test.
   // QFrame *frame = new QFrame;
   // frame->setFrameShape(QFrame::Box);  // 사각형 테두리
   // frame->setFrameShadow(QFrame::Plain);  // 단순한 테두리 스타일
   // frame->setLineWidth(2);  // 테두리 두께 설정
   // mainLayout->addWidget(frame);

   logView = new QListView(this);
   logView->setModel(logModel);
   gLayout->addWidget(logView, 0, 0);
    
   // [ Options ]
   auto *optionGroupBox = new QGroupBox();
   auto *optionGroupLayout = new QVBoxLayout();
   gLayout->addWidget(optionGroupBox, 0, 1);

   // 포트 선택 ComboBox
   createComboBoxLayout(optionGroupLayout, "COM port : ", portSelector, {});

   // Baudrate 선택 ComboBox
   createComboBoxLayout(optionGroupLayout, "Baud Rate : ", baudrateSelector,
                     {"2400", "4800", "9600", "14400", "19200", "38400", "57600", "115200"}, "115200");

   // DataBits 선택 ComboBox
   createComboBoxLayout(optionGroupLayout, "Data Bits : ", dataBitsSelector,
                     {"5", "6", "7", "8"}, "8");

   // Parity 선택 ComboBox
   createComboBoxLayout(optionGroupLayout, "Parity : ", paritySelector,
                     {"No Parity", "Even Parity", "Odd Parity", "Space Parity", "Mark Parity"}, "No Parity");

   // StopBits 선택 ComboBox
   createComboBoxLayout(optionGroupLayout, "Stop Bits : ", stopBitsSelector,
                     {"1", "1.5", "2"}, "1");

   // FlowControl 선택 ComboBox
   createComboBoxLayout(optionGroupLayout, "Flow Control : ", flowControlSelector,
                     {"No Flow Control", "Hardware Control", "Software Control"}, "No Flow Control");
   
   // [ Button ]
   // Open 및 Close 버튼 생성
   openButton = new QPushButton("Open", this);
   closeButton = new QPushButton("Close", this);

   // 버튼을 포함한 하단 레이아웃 생성
   auto *buttonLayout = new QHBoxLayout();
   buttonLayout->addWidget(openButton);
   buttonLayout->addWidget(closeButton);
   gLayout->addLayout(buttonLayout, 1, 0);

   // Clear 버튼 추가
   clearBtn = new QPushButton("Clear", this);
   clearBtn->setFixedSize(100, 25);
   gLayout->addWidget(clearBtn, 1, 1);

   // 초기 상태: Close 버튼 비활성화
   closeButton->setEnabled(false);

   // 시그널/슬롯 연결
   // widget
   connect(openButton, &QPushButton::clicked, this, &UartWidget::handleOpenButtonClicked);
   connect(closeButton, &QPushButton::clicked, this, &UartWidget::handleCloseButtonClicked);
   connect(clearBtn, &QPushButton::clicked, this, &UartWidget::allClear);
   // manager
   connect(uartManager, &UartManager::portOpened, this, &UartWidget::optionStateChanged);
   connect(uartManager, &UartManager::portClosed, this, &UartWidget::optionStateChanged);

   optionGroupBox->setLayout(optionGroupLayout);
   populateAvailablePorts();
}

// [option] 공통적인 ComboBox 레이아웃 생성 함수
void UartWidget::createComboBoxLayout(QVBoxLayout *parentLayout, const QString &labelText, QComboBox *&comboBox, const QStringList &items, const QString &defaultItem)
{
   QHBoxLayout *boxLayout = new QHBoxLayout();
   QLabel *label = new QLabel(labelText);
   comboBox = new QComboBox();
   comboBox->addItems(items);
   if (!defaultItem.isEmpty()) {
       comboBox->setCurrentText(defaultItem);
   }
   boxLayout->addWidget(label);
   boxLayout->addWidget(comboBox);
   parentLayout->addLayout(boxLayout);
}

void UartWidget::populateAvailablePorts()
{
   portSelector->clear();
   QStringList ports = uartManager->getAvailablePorts();

   if (ports.isEmpty())
   {
      portSelector->addItem("No Ports Available");
      logModel->appendRow(new QStandardItem("** No Ports Available **"));
   }
   else
   {
      portSelector->addItems(ports);
      logModel->appendRow(new QStandardItem("+++++  ++++++++++++++++++++++++++"));
      logModel->appendRow(new QStandardItem("▶️ There is currently an available COM port. "));
      logModel->appendRow(new QStandardItem("+++++  ++++++++++++++++++++++++++"));
      qDebug() << "Ready To Do Something. ";
   }
}

void UartWidget::handleOpenButtonClicked()
{
    // 포트 이름 및 Baudrate 가져오기
    QString selectedPortName = portSelector->currentText();
    int selectedBaudRate = baudrateSelector->currentText().toInt();

    // DataBits 매핑
    static const QMap<QString, QSerialPort::DataBits> dataBitsMap = {
        {"5", QSerialPort::Data5},
        {"6", QSerialPort::Data6},
        {"7", QSerialPort::Data7},
        {"8", QSerialPort::Data8}
    };
    QSerialPort::DataBits selectedDataBits = dataBitsMap.value(dataBitsSelector->currentText(), QSerialPort::Data8);

    // Parity 매핑
    static const QMap<QString, QSerialPort::Parity> parityMap = {
        {"No Parity", QSerialPort::NoParity},
        {"Even Parity", QSerialPort::EvenParity},
        {"Odd Parity", QSerialPort::OddParity},
        {"Space Parity", QSerialPort::SpaceParity},
        {"Mark Parity", QSerialPort::MarkParity}
    };
    QSerialPort::Parity selectedParity = parityMap.value(paritySelector->currentText(), QSerialPort::NoParity);

    // StopBits 매핑
    static const QMap<QString, QSerialPort::StopBits> stopBitsMap = {
        {"1", QSerialPort::OneStop},
        {"1.5", QSerialPort::OneAndHalfStop},
        {"2", QSerialPort::TwoStop}
    };
    QSerialPort::StopBits selectedStopBits = stopBitsMap.value(stopBitsSelector->currentText(), QSerialPort::OneStop);

    // FlowControl 매핑
    static const QMap<QString, QSerialPort::FlowControl> flowControlMap = {
        {"No Flow Control", QSerialPort::NoFlowControl},
        {"Hardware Control", QSerialPort::HardwareControl},
        {"Software Control", QSerialPort::SoftwareControl}
    };
    QSerialPort::FlowControl selectedFlowControl = flowControlMap.value(flowControlSelector->currentText(), QSerialPort::NoFlowControl);

    // UART 포트 열기
    uartManager->openSerialPort(selectedPortName, 
                                selectedBaudRate,
                                selectedDataBits,
                                selectedParity,
                                selectedStopBits,
                                selectedFlowControl);
}


void UartWidget::handleCloseButtonClicked()
{
   uartManager->closeSerialPort();
   logModel->appendRow(new QStandardItem("Closed port."));
}

void UartWidget::updateLog(const QString &data)
{
   logModel->appendRow(new QStandardItem(data));
}

void UartWidget::allClear()
{
   logModel->clear();
   //  optionStateChanged(SERIAL_PORT_STATE::SERIAL_PORT_NONE);
}

void UartWidget::optionStateChanged(int oState)
{
   qDebug() << "change combo & btn state " << oState;
   
   if (oState == SERIAL_PORT_STATE::SERIAL_PORT_OPEN)
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
