#-------------------------------------------------
#
# Project created by QtCreator 2015-09-23T11:10:05
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = Gui_Test_02
TEMPLATE = app

CONFIG += c++11

LIBS += -luhd\
        -lboost_system\
        -lboost_thread\
        -lboost_chrono\
        -lfftw3

SOURCES += main.cpp\
        mainwindow.cpp \
    qcustomplot.cpp \
    amc.cpp \
    fftgenerator.cpp \
    uhdmock.cpp \
    uhdread.cpp \
    efunction.cpp

HEADERS  += mainwindow.h \
    qcustomplot.h \
    amc.h \
    fftgenerator.h \
    sharedbuffer.h \
    sharedqvector.h \
    sharedvector.h \
    streamer.h \
    uhdmock.h \
    uhdread.h \
    streamfunction.h \
    efunction.h

FORMS    += mainwindow.ui

DISTFILES += \
    Gui_Test_02.pro.user
