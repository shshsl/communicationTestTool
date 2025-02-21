#-------------------------------------------------
#
# Project created by QtCreator 2025-02-13T17:38:16
#
#-------------------------------------------------

QT       += core gui qml quick serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = communicationTest
TEMPLATE = app


SOURCES += main.cpp\
                  src/testapp.cpp \
                  src/workerthread.cpp \
                  src/socketmanager.cpp \
                  src/uartmanager.cpp \
                  src/uartwidget.cpp \
    socketmanager.cpp

HEADERS  += \
                   include/testapp.h \
                   include/workerthread.h \
                   include/socketmanager.h \
                   include/uartmanager.h \
                   include/uartwidget.h \
    socketmanager.h

RESOURCES += \
                    qml.qrc

DISTFILES +=
