#ifndef UARTWIDGET_H
#define UARTWIDGET_H

#include <QWidget>
#include <QComboBox>
#include <QGroupBox>
#include <QGridLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListView>
#include <QStandardItemModel>
#include <QDebug>
#include <QLabel>
#include <QLineEdit>

#include "manager/include/uartmanager.h"

class UartWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UartWidget(QWidget *parent = nullptr);
    
    enum SERIAL_PORT_STATE
    {
        SERIAL_PORT_NONE = 0,
        SERIAL_PORT_OPEN = 1,
        SERIAL_PORT_CLOSE = 2,
    };
    Q_ENUM(SERIAL_PORT_STATE)

private slots:
    void handleOpenButtonClicked();
    void handleCloseButtonClicked();
    void updateLog(const QString &data);
    void handlePortStateChanged(int state);

private:
    void createComboBoxLayout(QVBoxLayout *parentLayout, const QString &labelText, QComboBox *&comboBox, const QStringList &items, const QString &defaultItem = "");
    void createDataInputLayout(QGridLayout *parentLayout, const QString &labelText, QLineEdit *&lineEdit);
    void populateAvailablePorts();
    void allClear();
    void optionStateChanged(int oState);
    
    // QString asciiToHex(const QString &ascii);
    // QString hexToAscii(const QString &hex);

private:
    UartManager *uartManager;
    
    QListView *logView;
    QStandardItemModel *logModel;
    QListView *controlView;
    QStandardItemModel *controlModel;
    QListView *dataView;
    QStandardItemModel *dataModel;
    
    QSerialPort *serialPort;                    // UART 포트 객체
    QPushButton *openButton;               // Open 버튼
    QPushButton *closeButton;               // Close 버튼
    QPushButton *logClearButton;
    QPushButton *dataClearButton;

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
    
    QLineEdit *dataInput;
    QPushButton *sendButton;
    QPushButton *toggleButton;
    
    bool isHexMode;
};

#endif // UARTWIDGET_H
