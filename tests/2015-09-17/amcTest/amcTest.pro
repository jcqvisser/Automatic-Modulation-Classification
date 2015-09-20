#-------------------------------------------------
#
# Project created by QtCreator 2015-09-17T09:28:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = amcTest
TEMPLATE = app

CONFIG += c++11

LIBS += -lfftw3


SOURCES += main.cpp\
        mainwindow.cpp \
    amc.cpp

HEADERS  += mainwindow.h \
    amc.h

FORMS    += mainwindow.ui
