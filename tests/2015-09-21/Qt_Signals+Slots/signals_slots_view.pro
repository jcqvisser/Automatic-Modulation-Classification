#-------------------------------------------------
#
# Project created by QtCreator 2015-09-21T12:48:53
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = signals_slots_view
TEMPLATE = app

CONFIG += c++11

LIBS += -luhd\
        -lboost_system\
        -lboost_thread\

SOURCES += main.cpp\
        mainwindow.cpp \
    qcustomplot.cpp \
    testemitter.cpp

HEADERS  += mainwindow.h \
    qcustomplot.h \
    testemitter.h

FORMS    += mainwindow.ui
