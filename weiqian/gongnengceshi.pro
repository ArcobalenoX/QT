#-------------------------------------------------
#
# Project created by QtCreator 2016-10-19T09:40:32
# 2017 1 19
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gongnengceshi
TEMPLATE = app
target.path = /home/asd
INSTALLS += target

SOURCES += main.cpp \
    canreadthread.cpp \
    canwidget.cpp \
    otherswidget.cpp \
    serialport.cpp \
    serialportreadthread.cpp \
    serialportwidget.cpp \
    watchdogwidget.cpp \
    weiqianfunctions.cpp \
    weiqiantools.cpp \
    com.cpp

HEADERS  += \
    canreadthread.h \
    canwidget.h \
    otherswidget.h \
    serialport.h \
    serialportreadthread.h \
    serialportwidget.h \
    watchdogwidget.h \
    weiqianfunctions.h \
    weiqiantools.h \
    com.h

DISTFILES += \
    weiqiantools.pri \
    gongnengceshi.pro.user

RESOURCES += \
    weiqiantoolsresrc.qrc

include(InputMethod/inputmethod.pri)
