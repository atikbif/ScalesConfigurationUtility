#-------------------------------------------------
#
# Project created by QtCreator 2016-04-14T10:33:54
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

RC_ICONS = scales.ico
VERSION = 1.0.0.1
QMAKE_TARGET_COMPANY = Kontel Co
QMAKE_TARGET_PRODUCT = ScalesConfig
QMAKE_TARGET_DESCRIPTION = Scales Configuration Utility
QMAKE_TARGET_COPYRIGHT = Kontel

TARGET = ScalesConfig
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    scalessettings.cpp \
    scales.cpp \
    modbusrequest.cpp \
    scanmanager.cpp \
    scanner.cpp

HEADERS  += mainwindow.h \
    scalessettings.h \
    scales.h \
    modbusrequest.h \
    scanmanager.h \
    scanner.h

FORMS    += mainwindow.ui
