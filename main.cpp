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
#include <QTabWidget>
#include <QLabel>
#include <QScreen>

#include "widget/include/uartwidget.h"
#include "widget/include/socketwidget.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QTabWidget tabWidget;

    UartWidget *uartTab = new UartWidget;
    SocketWidget *socketTab = new SocketWidget;

    // screen resolution.
    QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();

    int screenWidth = screenGeometry.width();
    int screenHeight = screenGeometry.height();

    // uartTab->setStyleSheet("background-color: red;");
    // socketTab->setStyleSheet("background-color: blue;");
    
    // setting tab bar style.
    tabWidget.setStyleSheet(
        "QTabBar::tab { background-color:rgb(24, 164, 235); }"
        "QTabBar::tab:selected { background-color:rgb(16, 233, 233); }"
        "QTabBar::tab:hover { background-color:rgb(230, 236, 134); }"
    );
    
    tabWidget.addTab(uartTab, "UART");
    tabWidget.addTab(socketTab, "SOCKET");
    tabWidget.setMinimumSize(screenWidth * 0.25, screenHeight * 0.4);
    tabWidget.resize(screenWidth * 0.3, screenHeight * 0.5);
    tabWidget.show();

    return app.exec();
}
