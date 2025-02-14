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
                  src/mainwindow.cpp \
                  src/testapp.cpp \

HEADERS  += include/mainwindow.h \
                   include/testapp.h \

RESOURCES += \
                    qml.qrc

DISTFILES +=
