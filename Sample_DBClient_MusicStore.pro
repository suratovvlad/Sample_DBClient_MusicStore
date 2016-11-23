#-------------------------------------------------
#
# Project created by QtCreator 2016-11-23T17:45:55
#
#-------------------------------------------------

QT       += core gui sql xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Sample_DBClient_MusicStore
TEMPLATE = app

CONFIG += console

SOURCES += main.cpp\
        MainWindow.cpp \
    ConnectionToDataBaseDialog.cpp \
    Core.cpp \
    SqlQueryManager.cpp \
    Wizard.cpp

HEADERS  += MainWindow.h \
    ConnectionToDataBaseDialog.h \
    Core.h \
    SqlQueryManager.h \
    Wizard.h

FORMS    += MainWindow.ui \
    ConnectionToDataBaseDialog.ui \
    Wizard.ui

RESOURCES += \
    dbclient.qrc
