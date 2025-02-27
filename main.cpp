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

#include <QWidget>
#include <QSysInfo>
#include <QDebug>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    // sysinfo.
    QString osName = QSysInfo::prettyProductName();
    QString osType = QSysInfo::kernelType();
    QString osVersion = QSysInfo::kernelVersion();
    qDebug() << "Current OS :   " << osName;
    qDebug() << "OS Type    :   " << osType;
    qDebug() << "OS Version :   " << osVersion;

    QTabWidget tabWidget;

    UartWidget *uartTab = new UartWidget;
    SocketWidget *socketTab = new SocketWidget;
    QWidget *testWidget = new QWidget;

    // screen resolution.
    QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();

    int screenWidth = screenGeometry.width();
    int screenHeight = screenGeometry.height();

    // uartTab->setStyleSheet("background-color: red;");
    // socketTab->setStyleSheet("background-color: blue;");
    
    // setting tab bar style.
    tabWidget.setStyleSheet(
        "QTabBar::tab {"
        "   background-color:rgb(24, 164, 235);"
        "   border: 1px solid white;"       // 기본 테두리 색상
        "   border-radius: 4px;"          // 테두리 모서리를 둥글게
        "   padding: 1px;"
        "   height: 20px;"
        "   width: 100px;"
        "}"
        "QTabBar::tab:selected { background-color:rgb(16, 233, 233); }"
        "QTabBar::tab:hover { "
        "   background-color:rgb(53, 57, 107); "
        "   color:rgb(255, 255, 255);"
        "}"
        "QTabBar::tab { width: 100px; }"
    );
    
    tabWidget.addTab(uartTab, "UART");
    tabWidget.addTab(socketTab, "SOCKET");
    tabWidget.addTab(testWidget, "TEST");//test
    tabWidget.setMinimumSize(screenWidth * 0.25, screenHeight * 0.4);
    tabWidget.resize(screenWidth * 0.3, screenHeight * 0.7);
    tabWidget.show();

    return app.exec();
}
