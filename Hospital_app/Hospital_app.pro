#-------------------------------------------------
#
# Project created by QtCreator 2021-04-27T17:02:23
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Hospital_app
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    paciente.cpp

HEADERS  += mainwindow.h \
    paciente.h

FORMS    += mainwindow.ui

RESOURCES += \
    background.qrc

DISTFILES +=
