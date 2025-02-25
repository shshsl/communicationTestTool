#-------------------------------------------------
#
# Project created by QtCreator 2025-02-13T17:38:16
#
#-------------------------------------------------

QT       += core gui qml quick serialport
CONFIG   += c++17

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = communicationTest
TEMPLATE = app


SOURCES += main.cpp\
                  widget/src/testapp.cpp \
                  widget/src/uartwidget.cpp \
                  widget/src/socketwidget.cpp \
                  manager/src/workerthread.cpp \
                  manager/src/socketmanager.cpp \
                  manager/src/uartmanager.cpp \

HEADERS  += \
                   widget/include/testapp.h \
                   widget/include/uartwidget.h \
                   widget/include/socketwidget.h \
                   manager/include/workerthread.h \
                   manager/include/socketmanager.h \
                   manager/include/uartmanager.h \
    communicationenum.h

RESOURCES += \
                    qml.qrc

DISTFILES +=
