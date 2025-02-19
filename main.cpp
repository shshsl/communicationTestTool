///=====================================================================================
/// exec for qml.
 /*
//#include <QGuiApplication>
//#include <QQmlApplicationEngine>
//#include <QQmlContext>
//#include "include/testapp.h"

//int main(int argc, char *argv[]) {
//    QGuiApplication app(argc, argv);

//    QQmlApplicationEngine engine;

//    // TestApp 객체 생성 및 QML에 등록
//    TestApp testApp;
//    engine.rootContext()->setContextProperty("testApp", &testApp);

//    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
//    if (engine.rootObjects().isEmpty())
//        return -1;

//    return app.exec();
//}
*/

///=====================================================================================
/// exec for cpp.
///
#include <QApplication>
#include "include/uartwidget.h"

#include <QTabWidget>
#include <QLabel>
#include <QVBoxLayout>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QTabWidget tabWidget;

    // Uart Tab
    UartWidget *uartTab = new UartWidget;
    QVBoxLayout *uartLayout = new QVBoxLayout;
//    uartLayout->addWidget(new QLabel("This is [ UART ] Tab"));
    uartTab->setLayout(uartLayout);

    // Socket Tab
    QWidget *socketTab = new QWidget;
    QVBoxLayout *socketLayout = new QVBoxLayout;
//    socketLayout->addWidget(new QLabel("This is [ SOCKET ] Tab"));
    socketTab->setLayout(socketLayout);

    tabWidget.addTab(uartTab, "UART");
    tabWidget.addTab(socketTab, "SOCKET");
    tabWidget.resize(800, 600);
    tabWidget.show();

    return app.exec();
}
