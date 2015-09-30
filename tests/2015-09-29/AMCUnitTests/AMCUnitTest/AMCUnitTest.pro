#-------------------------------------------------
#
# Project created by QtCreator 2015-09-29T14:50:09
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_amcunittest
CONFIG   += console \
            c++11
CONFIG   -= app_bundle

LIBS += -lfftw3 \
        -lboost_thread \
        -lboost_system

TEMPLATE = app


SOURCES += tst_amcunittest.cpp \
    amc.cpp \
    featureextractor.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += \
    amc.h \
    featureextractor.h \
    sharedbuffer.h \
    sharedvector.h
