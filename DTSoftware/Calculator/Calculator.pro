#-------------------------------------------------
#
# Project created by QtCreator 2015-11-08T15:30:49
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++11

TARGET = Calculator
TEMPLATE = app


SOURCES += main.cpp \
    QCalculatorUI.cpp \
    QCalculatorDec.cpp \
    QCalculator.cpp

HEADERS  += \
    QCalculatorUI.h \
    QCalculatorDec.h \
    ICalculator.h \
    QCalculator.h
