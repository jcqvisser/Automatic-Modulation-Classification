#-------------------------------------------------
#
# Project created by QtCreator 2015-09-09T14:27:26
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = j_uhdtest_01
TEMPLATE = app

LIBS += -lfftw3 \
        -lboost_system \
        -lboost_thread \
        -luhd



SOURCES += main.cpp\
        mainwindow.cpp \
    qcustomplot.cpp

HEADERS  += mainwindow.h \
    qcustomplot.h

FORMS    += mainwindow.ui
