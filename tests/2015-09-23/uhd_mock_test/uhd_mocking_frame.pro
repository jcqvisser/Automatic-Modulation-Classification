#-------------------------------------------------
#
# Project created by QtCreator 2015-09-22T08:45:45
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = uhd_mocking_frame
TEMPLATE = app

CONFIG += c++11

LIBS += -luhd\
        -lboost_system\
        -lboost_thread\
        -lfftw3

SOURCES += main.cpp\
        mainwindow.cpp \
    qcustomplot.cpp \
    uhdread.cpp \
    uhdmock.cpp \
    amc.cpp \
    fftgenerator.cpp

HEADERS  += mainwindow.h \
    qcustomplot.h \
    streamer.h \
    sharedbuffer.h \
    uhdread.h \
    uhdmock.h \
    amc.h \
    fftgenerator.h \
    sharedvector.h \
    sharedqvector.h

FORMS    += mainwindow.ui
