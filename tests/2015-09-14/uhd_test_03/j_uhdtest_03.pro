#-------------------------------------------------
#
# Project created by QtCreator 2015-09-11T15:27:29
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = j_uhdtest_03
TEMPLATE = app

LIBS += -luhd\
        -lboost_system\
        -lboost_thread\

SOURCES += main.cpp\
        mainwindow.cpp \
    qcustomplot.cpp

HEADERS  += mainwindow.h \
    qcustomplot.h

FORMS    += mainwindow.ui
