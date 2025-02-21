#include "manager/include/workerthread.h"

WorkerThread::WorkerThread(QThread *parent)
{

}

//WorkerThread::~SerialWorker()
//{

//}



/*
//// 비동기 UI 업데이트.
void MainWindow::showProgressDialog() {
    // QProgressDialog 생성
    QProgressDialog* progress = new QProgressDialog("포트를 여는 중입니다...", "취소", 0, 0, this);
    progress->setWindowModality(Qt::WindowModal);
    progress->setCancelButton(nullptr); // 취소 버튼 숨김
    progress->show();

    // Worker 스레드 생성
    QThread* workThread = new QThread(this);
    serialPort->moveToThread(workThread);

    connect(workThread, &QThread::started, [=]() {
        bool success = serialPort->open(QIODevice::ReadOnly);

        emit threadFinished(success); // 결과 전달
        workThread->quit();
    });

    connect(workThread, &QThread::finished, this, [=]() {
        progress->close();

        if (serialPort->isOpen())
        {
            connect(serialPort, &QSerialPort::readyRead, this, &MainWindow::readSerialData);

            logModel->appendRow(new QStandardItem("> Opened port: [ " + m_selectedPort + " ] at [ " + m_selectedBaudRate + " ] baud"));
            qDebug() << "Opened port:" << m_selectedPort << "at" << m_selectedBaudRate << "baud";

            optionStateChanged(SERIAL_PORT_STATE::SERIAL_PORT_CLOSE);   // toggle
        }
        else
        {
            QMessageBox::critical(this, "실패", "포트를 열 수 없습니다.");

            logModel->appendRow(new QStandardItem("** !.!. Failed to open port: [ " + m_selectedPort + " ]"));
            qDebug() << "Failed to open port:" << serialPort->errorString();

            optionStateChanged(SERIAL_PORT_STATE::SERIAL_PORT_OPEN);    // toggle
        }

        workThread->deleteLater();
//        serialPort->deleteLater();    // serialport는 closebutton 때만 제거.
        progress->deleteLater();
    });

    workThread->start();
}
*/
