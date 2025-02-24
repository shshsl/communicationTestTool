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
    ~UartWidget();
    
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
    void createDataInputLayout(QGridLayout *parentLayout, QLineEdit *&lineEdit, const int row = 0);
    void populateAvailablePorts();
    void allClear();
    void dataClear();
    void optionStateChanged(int oState);
    
    // QString asciiToHex(const QString &ascii);
    // QString hexToAscii(const QString &hex);

private:
    UartManager *uartManager;
    
    QListView *logView;
    QStandardItemModel *logModel;
    QListView *dataView;
    QStandardItemModel *dataModel;
    QListView *sendDataView;
    QStandardItemModel *sendDataModel;
    
    QSerialPort *serialPort;
    QPushButton *openButton;
    QPushButton *closeButton;
    QPushButton *logClearButton;
    QPushButton *dataClearButton;

    QComboBox *portSelector;
    QComboBox *baudrateSelector;
    QComboBox *dataBitsSelector;
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
    QVector<QPushButton *> sendButtons;
    QVector<QPushButton *> toggleButtons;
    
    int m_nLayoutRow = 0;
    int m_nLayoutColumn = 0;
};

#endif // UARTWIDGET_H
