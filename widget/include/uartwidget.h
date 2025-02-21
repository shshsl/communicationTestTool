// #ifndef UARTWIDGET_H
// #define UARTWIDGET_H

// #include <QApplication>
// #include <QGridLayout>
// #include <QListView>
// #include <QGroupBox>
// #include <QComboBox>
// #include <QLabel>
// #include <QPushButton>
// #include <QStandardItemModel>
// #include <QSerialPort>
// #include <QSerialPortInfo>
// #include <QDebug>
// #include <QMessageBox>
// #include <QProgressDialog>
// //
// #include <QThread>

// class UartWidget: public QWidget {
//     Q_OBJECT

// public:
//     explicit UartWidget(QWidget *parent = nullptr);
//     ~UartWidget();

//     enum SERIAL_PORT_STATE
//     {
//             SERIAL_PORT_NONE = 0,
//             SERIAL_PORT_OPEN = 1,
//             SERIAL_PORT_CLOSE = 2,
//     };
//     Q_ENUM(SERIAL_PORT_STATE)

// private slots:
//     void readSerialData();  // UART 데이터를 읽는 슬롯
//     void openSerialPort();  // Open 버튼 클릭 시 호출
//     void closeSerialPort(); // Close 버튼 클릭 시 호출

// signals:
//     void threadFinished(bool success);

// private:
//     QSerialPort *serialPort;                    // UART 포트 객체
//     QStandardItemModel *logModel;       // QListView의 데이터 모델
//     QPushButton *openButton;               // Open 버튼
//     QPushButton *closeButton;               // Close 버튼
//     QPushButton *clearBtn;

//     QComboBox *portSelector;               // 포트 선택 ComboBox
//     QComboBox *baudrateSelector;        // Baudrate 선택 ComboBox
//     QComboBox *dataBitsSelector;         // DataBits 선택 ComboBox
//     QComboBox *paritySelector;
//     QComboBox *stopBitsSelector;
//     QComboBox *flowControlSelector;

//     QLabel *portLabel;
//     QLabel *baudLabel;
//     QLabel *dataBitsLabel;
//     QLabel *parityLabel;
//     QLabel *stopBitsLabel;
//     QLabel *flowLabel;

//     QString m_selectedPort;
//     QString m_selectedBaudRate;

//     void populateAvailablePorts();   // 사용 가능한 포트를 검색하고 ComboBox에 추가
//     void logClear();
//     void optionStateChanged(int oState);
//     void showProgressDialog();
// };

// #endif // UARTWIDGET_H


#ifndef UARTWIDGET_H
#define UARTWIDGET_H

#include <QWidget>
#include "manager/include/uartmanager.h"

class QComboBox;
class QListView;
class QStandardItemModel;

class UartWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UartWidget(QWidget *parent = nullptr);

private slots:
    void handleOpenButtonClicked();
    void handleCloseButtonClicked();
    void updateLog(const QString &data);

private:
    void populateAvailablePorts();

private:
    UartManager *uartManager;

    QComboBox *portSelector;
    QComboBox *baudrateSelector;
    
    QListView *logView;
    QStandardItemModel *logModel;
};

#endif // UARTWIDGET_H
