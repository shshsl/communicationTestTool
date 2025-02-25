#include "widget/include/uartwidget.h"

UartWidget::UartWidget(QWidget *parent)
   : QWidget(parent), uartManager(new UartManager(this)),
   logModel(new QStandardItemModel(this))
{
   // UI 구성
   auto *gLayout = new QGridLayout(this);
   // setLayout(gLayout);
   
   // // QFrame 생성 및 설정 - layout 위치 확인용 test.
   // QFrame *frame = new QFrame;
   // frame->setFrameShape(QFrame::Box);  // 사각형 테두리
   // frame->setFrameShadow(QFrame::Plain);  // 단순한 테두리 스타일
   // frame->setLineWidth(2);  // 테두리 두께 설정
   // gLayout->addWidget(frame);

   logView = new QListView(this);
   logView->setModel(logModel);
   logView->setEditTriggers(QAbstractItemView::NoEditTriggers);   // disable editing
   gLayout->addWidget(logView, m_nLayoutRow, 0);
       
   // [ Options ]
   auto *optionGroupBox = new QGroupBox();
   optionGroupBox->setTitle("[ Setting Options ]");
   auto *optionGroupLayout = new QVBoxLayout();
   gLayout->addWidget(optionGroupBox, 0, 1);

   createComboBoxLayout(optionGroupLayout, "Device : ", portSelector, {});

   createComboBoxLayout(optionGroupLayout, "Baudrate : ", baudrateSelector,
                     {"2400", "4800", "9600", "14400", "19200", "38400", "57600", "115200", "460800", "921600", "230400"}, "115200");

   createComboBoxLayout(optionGroupLayout, "Data Bits : ", dataBitsSelector,
                     {"5", "6", "7", "8"}, "8");

   createComboBoxLayout(optionGroupLayout, "Parity : ", paritySelector,
                     {"No Parity", "Even Parity", "Odd Parity", "Space Parity", "Mark Parity"}, "No Parity");

   createComboBoxLayout(optionGroupLayout, "Stop Bits : ", stopBitsSelector,
                     {"1", "1.5", "2"}, "1");

   createComboBoxLayout(optionGroupLayout, "Flow Control : ", flowControlSelector,
                     {"No Flow Control", "Hardware Control", "Software Control"}, "No Flow Control");
   
   // [ Button ]
   openButton = new QPushButton("Open", this);
   closeButton = new QPushButton("Close", this);

   auto *buttonLayout = new QHBoxLayout();
   buttonLayout->addWidget(openButton);
   buttonLayout->addWidget(closeButton);
   optionGroupLayout->addLayout(buttonLayout);

   logClearButton = new QPushButton("Log Clear", this);
   logClearButton->setFixedSize(100, 25);
   gLayout->addWidget(logClearButton, m_nLayoutRow + 1, 1, Qt::AlignRight);

   // [ data send ]
   for(int i = 0 ; i < 8 ; i++) {
      createDataInputLayout(gLayout, dataInput, i);
      // m_nLayoutRow + 2 + i
   }

   sendDataView = new QTextEdit(this);   // TX 데이터 뷰
   sendDataViewLabel = new QLabel("▶️ Send Data :");
   sendDataView->setReadOnly(true);
   sendDataView->setPlaceholderText("Send Data to Device...");
   dataView = new QTextEdit(this);       // RX 데이터 뷰
   dataViewLabel = new QLabel("▶️ Receive Data :");
   dataView->setReadOnly(true);
   dataView->setPlaceholderText("Receive Data from Device...");
   sendDataViewLabel->setContentsMargins(0, 10, 0, 0);   //left, top, right, bottom
   dataViewLabel->setContentsMargins(0, 10, 0, 0);       //left, top, right, bottom
   
   gLayout->addWidget(sendDataViewLabel, m_nLayoutRow + 1, 0);
   gLayout->addWidget(sendDataView, m_nLayoutRow + 2, 0);
   gLayout->addWidget(dataViewLabel, m_nLayoutRow + 1, 1);
   gLayout->addWidget(dataView, m_nLayoutRow + 2, 1);
   m_nLayoutRow = m_nLayoutRow + 2;
   
   dataClearButton = new QPushButton("Data Clear", this);
   dataClearButton->setFixedSize(100, 25);
   gLayout->addWidget(dataClearButton, m_nLayoutRow + 1, 1, Qt::AlignRight);

   // init state: disable buttons.
   closeButton->setEnabled(false);
   for (QPushButton *button : sendButtons) {
      button->setEnabled(false);
   }

   // coonect signal/slot
   // widget
   connect(openButton, &QPushButton::clicked, this, &UartWidget::handleOpenButtonClicked);
   connect(closeButton, &QPushButton::clicked, this, &UartWidget::handleCloseButtonClicked);
   connect(logClearButton, &QPushButton::clicked, this, &UartWidget::allClear);
   connect(dataClearButton, &QPushButton::clicked, this, &UartWidget::dataClear);
   // connect(logModel, &QStandardItemModel::rowsInserted, this, [=](const QModelIndex &parent, int first, int last) {  // find listview index.
   //    for (int row = first; row <= last; ++row) {
   //        QModelIndex index = logModel->index(row, 0, parent); // 첫 번째 열 기준
   //        qDebug() << "New item added at Row:" << row << "Column:" << index.column();
   //    }
   // });
   // manager
   connect(uartManager, &UartManager::dataReceived, this, &UartWidget::updateReceivedData);
   connect(uartManager, &UartManager::dataSend, this, &UartWidget::updateSendData);
   connect(uartManager, &UartManager::portOpened, this, &UartWidget::handlePortStateChanged);
   connect(uartManager, &UartManager::portClosed, this, &UartWidget::handlePortStateChanged);
   connect(uartManager, &UartManager::portFailedToOpen, this, &UartWidget::handlePortStateChanged);

   optionGroupBox->setLayout(optionGroupLayout);
   populateAvailablePorts();
}

UartWidget::~UartWidget()
{
   // widget
   disconnect(openButton, &QPushButton::clicked, this, &UartWidget::handleOpenButtonClicked);
   disconnect(closeButton, &QPushButton::clicked, this, &UartWidget::handleCloseButtonClicked);
   disconnect(logClearButton, &QPushButton::clicked, this, &UartWidget::allClear);
   disconnect(dataClearButton, &QPushButton::clicked, this, &UartWidget::dataClear);
   // manager
   disconnect(uartManager, &UartManager::portOpened, this, &UartWidget::handlePortStateChanged);
   disconnect(uartManager, &UartManager::portClosed, this, &UartWidget::handlePortStateChanged);
   disconnect(uartManager, &UartManager::portFailedToOpen, this, &UartWidget::handlePortStateChanged);
   
   if (this->layout() != nullptr) {
      delete this->layout();
   }
   delete uartManager;
}

// [option] ComboBox common layout
void UartWidget::createComboBoxLayout(QVBoxLayout *parentLayout, const QString &labelText, QComboBox *&comboBox, const QStringList &items, const QString &defaultItem)
{
   QHBoxLayout *boxLayout = new QHBoxLayout();
   QLabel *label = new QLabel(labelText);
   bool isEditable = false;
   if(comboBox == portSelector || comboBox == baudrateSelector) {
       isEditable = true;
   }
   comboBox = new QComboBox();
   comboBox->setEditable(isEditable);
   comboBox->setInsertPolicy(QComboBox::NoInsert); // Prevent adding new items automatically   
   comboBox->addItems(items);
   if (!defaultItem.isEmpty()) {
       comboBox->setCurrentText(defaultItem);
   }
   boxLayout->addWidget(label);
   boxLayout->addWidget(comboBox);
   parentLayout->addLayout(boxLayout);
}

void UartWidget::createDataInputLayout(QGridLayout *parentLayout, QLineEdit *&lineEdit, const int row)
{
   QHBoxLayout *boxLayout = new QHBoxLayout();
   // Create and customize QLabel
   QLabel *label = new QLabel(QString("Data (%1): ").arg(row), this);
   lineEdit = new QLineEdit();
   lineEdit->setMaxLength(64); // 최대 입력 길이 제한 (64바이트)
   lineEdit->setPlaceholderText("Enter data here...");
   lineEdit->setClearButtonEnabled(true); // Adds a clear button inside the line edit
   // lineEdit->setValidator(new QRegExpValidator(QRegExp("[0-9A-Fa-f ]*"), this)); // Example: HEX input only
   
   // Create and customize send QPushButton
   QPushButton *sendButton = new QPushButton(this);
   sendButtons.append(sendButton); // Store in container
   sendButton->setText("Send");
   sendButton->setIcon(QIcon(":/icons/send.png")); // Add an icon (replace with your icon path)
   sendButton->setStyleSheet("background-color: lightblue; border-radius: 5px;"); // Styling
   connect(sendButton, &QPushButton::clicked, this, [=]() {
      QString data = lineEdit->text();
      qDebug() << "Send button clicked for row" << row + 1 << "with data:" << data;
      uartManager->writeSerialData(data);
   });
   
   // Create and customize toggle QPushButton
   QPushButton *toggleButton = new QPushButton(this);
   toggleButtons.append(toggleButton); // Store in container
   toggleButton->setText("ASCII");
   toggleButton->setStyleSheet("color: blue;");
   toggleButton->setCheckable(true); // Make it toggleable
//   toggleButton->setStyleSheet("background-color: lightgray; border-radius: 5px;");
   connect(toggleButton, &QPushButton::toggled, this, [=](bool checked) {
      toggleButton->setText(checked ? "HEX" : "ASCII");
      qDebug() << "Toggle button clicked for row" << row + 1 << "state:" << (checked ? "HEX" : "ASCII");
      toggleButtons[row]->setStyleSheet(checked ? "color: green;" : "color: blue;");
      if (checked)
      {
         lineEdit->setValidator(new QRegExpValidator(QRegExp("[0-9A-Fa-f ]*"), this));// HEX: 16진수 입력만 허용
      }
      else
      {
         lineEdit->setValidator(nullptr);// ASCII: Validator 제거 (모든 입력 허용)
      }
   });
   
   boxLayout->addWidget(label);
   boxLayout->addWidget(lineEdit);
   boxLayout->addWidget(toggleButton);
   boxLayout->addWidget(sendButton);
   parentLayout->addLayout(boxLayout, row + 2, 0, 1, 2);
   
   m_nLayoutRow = row + 2;
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
   // logModel->appendRow(new QStandardItem("Closed port."));
}

void UartWidget::updateReceivedData(const QString &data)
{
   // TX data
   // qDebug() << "*********** updateReceivedData:" << data;//test log
   if (dataView) {
      QString timestamp = QDateTime::currentDateTime().toString("yy/MM/dd HH:mm:ss");
      dataView->append(QString("[%1] %2").arg(timestamp).arg(data));
   }
}

void UartWidget::updateSendData(const QString &data)
{
   // RX data
   // qDebug() << "*********** updateSendData:" << data;//test log
   if (dataView) {
      QString timestamp = QDateTime::currentDateTime().toString("yy/MM/dd HH:mm:ss");
      sendDataView->append(QString("[%1] %2").arg(timestamp).arg(data));
   }
}

void UartWidget::allClear()
{
   logModel->clear();
   //  optionStateChanged(SERIAL_PORT_STATE::SERIAL_PORT_NONE);
}

void UartWidget::dataClear()
{
   sendDataView->clear();
   dataView->clear();
}

void UartWidget::handlePortStateChanged(int state)
{
   QString logMessage;

   switch (state)
   {
   case Communication::Uart::SerialPortState::SERIAL_PORT_OPEN:
      logMessage = "Serial port opened successfully.";
      break;
   case Communication::Uart::SerialPortState::SERIAL_PORT_CLOSE:
      logMessage = "Serial port closed.";
      break;
   case Communication::Uart::SerialPortState::SERIAL_PORT_NONE:
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
   
   if (oState == Communication::Uart::SerialPortState::SERIAL_PORT_OPEN)
   {
      openButton->setEnabled(false);
      closeButton->setEnabled(true);
      portSelector->setEnabled(false);
      baudrateSelector->setEnabled(false);
      dataBitsSelector->setEnabled(false);
      paritySelector->setEnabled(false);
      stopBitsSelector->setEnabled(false);
      flowControlSelector->setEnabled(false);
      for (QPushButton *button : sendButtons) {
         button->setEnabled(true);
      }
   }
   else
   {
      openButton->setEnabled(true);
      closeButton->setEnabled(false);
      portSelector->setEnabled(true);
      baudrateSelector->setEnabled(true);
      dataBitsSelector->setEnabled(true);
      paritySelector->setEnabled(true);
      stopBitsSelector->setEnabled(true);
      flowControlSelector->setEnabled(true);
      for (QPushButton *button : sendButtons) {
         button->setEnabled(false);
      }
   }
   
   // QCoreApplication::processEvents(); // UI 강제 갱신
}

// QString UartWidget::asciiToHex(const QString &ascii) {
//    return QString::fromLatin1(ascii.toLatin1().toHex(' ').toUpper()); // HEX 변환
// }

// QString UartWidget::hexToAscii(const QString &hex) {
//    QByteArray byteArray = QByteArray::fromHex(hex.toLatin1());
//    return QString::fromLatin1(byteArray); // ASCII 변환
// }
