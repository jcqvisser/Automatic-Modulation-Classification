#-------------------------------------------------
#
# Project created by QtCreator 2015-09-22T14:58:32
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = AMCThreading
CONFIG   += console \
            c++11

CONFIG   -= app_bundle

LIBS += -lfftw3 \
        -lboost_thread \
        -lboost_system


TEMPLATE = app


SOURCES += main.cpp \
    amc.cpp \
    featureextractor.cpp

HEADERS += \
    amc.h \
    featureextractor.h \
    sharedbuffer.h
