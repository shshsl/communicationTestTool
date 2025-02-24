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

/////////
#include "widget/include/uartwidget.h"
#include <QLineEdit>

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
   // gLayout->addWidget(frame);

   logView = new QListView(this);
   logView->setModel(logModel);
   gLayout->addWidget(logView, 0, 0);
   
   controlView = new QListView(this);
   controlView->setModel(controlModel);
   gLayout->addWidget(controlView, 2, 0);
   
   dataView = new QListView(this);
   dataView->setModel(dataModel);
   gLayout->addWidget(dataView, 3, 0);
    
   // [ Options ]
   auto *optionGroupBox = new QGroupBox();
   auto *optionGroupLayout = new QVBoxLayout();
   gLayout->addWidget(optionGroupBox, 0, 1);

   // 포트 선택 ComboBox
   createComboBoxLayout(optionGroupLayout, "Device : ", portSelector, {});

   // Baudrate 선택 ComboBox
   createComboBoxLayout(optionGroupLayout, "Baudrate : ", baudrateSelector,
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
   optionGroupLayout->addLayout(buttonLayout);

   // Clear 버튼 추가
   logClearButton = new QPushButton("Log Clear", this);
   logClearButton->setFixedSize(100, 25);
   gLayout->addWidget(logClearButton, 1, 1);
   
   dataClearButton = new QPushButton("Data Clear", this);
   dataClearButton->setFixedSize(100, 25);
   gLayout->addWidget(dataClearButton, 3, 1);

   // 초기 상태: Close 버튼 비활성화
   closeButton->setEnabled(false);

   // coonect signal/slot
   // widget
   connect(openButton, &QPushButton::clicked, this, &UartWidget::handleOpenButtonClicked);
   connect(closeButton, &QPushButton::clicked, this, &UartWidget::handleCloseButtonClicked);
   connect(logClearButton, &QPushButton::clicked, this, &UartWidget::allClear);
   // manager
   connect(uartManager, &UartManager::portOpened, this, &UartWidget::handlePortStateChanged);
   connect(uartManager, &UartManager::portClosed, this, &UartWidget::handlePortStateChanged);
   connect(uartManager, &UartManager::portFailedToOpen, this, &UartWidget::handlePortStateChanged);

   optionGroupBox->setLayout(optionGroupLayout);
   populateAvailablePorts();
}

// [option] ComboBox common layout
void UartWidget::createComboBoxLayout(QVBoxLayout *parentLayout, const QString &labelText, QComboBox *&comboBox, const QStringList &items, const QString &defaultItem)
{
   QHBoxLayout *boxLayout = new QHBoxLayout();
   QLabel *label = new QLabel(labelText);
   comboBox = new QComboBox();
   comboBox->setEditable(true);
   comboBox->setInsertPolicy(QComboBox::NoInsert); // Prevent adding new items automatically
   
   // // Optional: Customize line edit if needed   
   // //(if option is empty, it will be shown as a placeholder text)
   // QLineEdit *lineEdit = comboBox->lineEdit();
   // lineEdit->setPlaceholderText("Type or select an option");
   
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
   dataModel->appendRow(new QStandardItem(data));
}

void UartWidget::allClear()
{
   logModel->clear();
   //  optionStateChanged(SERIAL_PORT_STATE::SERIAL_PORT_NONE);
}

void UartWidget::handlePortStateChanged(int state)
{
    QString logMessage;

    switch (state)
    {
    case SERIAL_PORT_STATE::SERIAL_PORT_OPEN:
        logMessage = "Serial port opened successfully.";
        break;
    case SERIAL_PORT_STATE::SERIAL_PORT_CLOSE:
        logMessage = "Serial port closed.";
        break;
    case SERIAL_PORT_STATE::SERIAL_PORT_NONE:
        logMessage = "Failed to open serial port.";
        break;
    default:
        logMessage = "Unknown serial port state.";
        break;
    }
    
    optionStateChanged(state);

    // Append the message to the log model
    logModel->appendRow(new QStandardItem(logMessage));
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
