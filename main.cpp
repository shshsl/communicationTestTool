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
/// exec for qml.
///
#include <QApplication>
#include "include/mainwindow.h"

#include <QTabWidget>
#include <QLabel>
#include <QVBoxLayout>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // QTabWidget 생성
    QTabWidget tabWidget;

    // A 탭 생성
    MainWindow *uartTab = new MainWindow;
    QVBoxLayout *uartLayout = new QVBoxLayout;
//    uartLayout->addWidget(new QLabel("This is [ UART ] Tab"));
    uartTab->setLayout(uartLayout);

    // B 탭 생성
    QWidget *socketTab = new QWidget;
    QVBoxLayout *socketLayout = new QVBoxLayout;
//    socketLayout->addWidget(new QLabel("This is [ SOCKET ] Tab"));
    socketTab->setLayout(socketLayout);

    // 탭 추가
    tabWidget.addTab(uartTab, "UART");
    tabWidget.addTab(socketTab, "SOCKET");

//    MainWindow window;
//    window.resize(800, 600);
//    window.show();

    // QTabWidget 표시
    tabWidget.resize(800, 600);
    tabWidget.show();

    return app.exec();
}
