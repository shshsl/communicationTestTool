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
#widget
                  widget/src/testapp.cpp \
                  widget/src/uartwidget.cpp \
                  widget/src/socketwidget.cpp \
#mgr
                  manager/src/workerthread.cpp \
                  manager/src/socketmanager.cpp \
                  manager/src/uartmanager.cpp \

HEADERS  += \
#widget
                   widget/include/testapp.h \
                   widget/include/uartwidget.h \
                   widget/include/socketwidget.h \
#mgr
                   manager/include/workerthread.h \
                   manager/include/socketmanager.h \
                   manager/include/uartmanager.h \

RESOURCES += \
                    qml.qrc

DISTFILES +=
