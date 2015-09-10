#-------------------------------------------------
#
# Project created by QtCreator 2015-09-07T12:07:14
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

LIBS += -luhd \
        -lboost_system \
        -lboost_thread \
        -lfftw3

TARGET = QCustomPlot-Test
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qcustomplot.cpp


HEADERS  += \
        mainwindow.h \
    qcustomplot.h


FORMS    += mainwindow.ui
