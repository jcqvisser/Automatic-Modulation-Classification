#-------------------------------------------------
#
# Project created by QtCreator 2015-10-04T19:14:47
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = FileWriterTest
CONFIG   += console\
            c++11
CONFIG   -= app_bundle

INCLUDEPATH += ../../../../src/

TEMPLATE = app

LIBS += -luhd\
        -lboost_system\
        -lboost_thread\
        -lboost_chrono\
        -lboost_date_time\
        -lfftw3\
        -lfftw3f\
        -lliquid



SOURCES += main.cpp \
    ../../../../src/amc.cpp \
    ../../../../src/filewriter.cpp

HEADERS += \
    ../../../../src/amc.h \
    ../../../../src/filewriter.h \
    ../../../../src/sharedbuffer.h
