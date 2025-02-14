#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QStandardItemModel>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

//    enum SERIAL_PORT_STATE
//    {
//            SERIAL_PORT_OPEN = 0,
//            SERIAL_PORT_OPEN = 1,
//            SERIAL_PORT_CLOSE = 2,
//    };
//    Q_ENUM(SERIAL_PORT_STATE)

private slots:
    void readSerialData();  // UART 데이터를 읽는 슬롯
    void openSerialPort();  // Open 버튼 클릭 시 호출
    void closeSerialPort(); // Close 버튼 클릭 시 호출

private:
    QSerialPort *serialPort;                    // UART 포트 객체
    QStandardItemModel *logModel;       // QListView의 데이터 모델
    QPushButton *openButton;               // Open 버튼
    QPushButton *closeButton;               // Close 버튼
    QPushButton *clearBtn;

    QComboBox *portSelector;               // 포트 선택 ComboBox
    QComboBox *baudrateSelector;        // Baudrate 선택 ComboBox
    QComboBox *dataBitsSelector;         // DataBits 선택 ComboBox
    QComboBox *paritySelector;
    QComboBox *stopBitsSelector;
    QComboBox *flowControlSelector;

    QLabel *portLabel;
    QLabel *baudLabel;
    QLabel *dataBitsLabel;
    QLabel *parityLabel;
    QLabel *stopBitsLabel;
    QLabel *flowLabel;

    int m_nOpen = 0;

    void populateAvailablePorts();   // 사용 가능한 포트를 검색하고 ComboBox에 추가
    void logClear();
    void optionStateChanged(int oState);
};

#endif // MAINWINDOW_H
