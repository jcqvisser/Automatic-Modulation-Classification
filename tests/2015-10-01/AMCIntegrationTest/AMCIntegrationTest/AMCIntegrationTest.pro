#-------------------------------------------------
#
# Project created by QtCreator 2015-10-01T20:20:39
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = AMCIntegrationTest
CONFIG   += console \
            c++11
CONFIG   -= app_bundle

TEMPLATE = app

LIBS += -luhd\
        -lboost_system\
        -lboost_thread\
        -lboost_chrono\
        -lboost_random\
        -lfftw3\
        -lfftw3f\
        -lliquid


SOURCES += main.cpp \
    amc.cpp \
    amfunction.cpp \
    featureextractor.cpp \
    uhdmock.cpp \
    datasink.cpp \
    cosfunction.cpp

DISTFILES +=

HEADERS += \
    amc.h \
    amfunction.h \
    featureextractor.h \
    sharedvector.h \
    streamer.h \
    uhdmock.h \
    sharedbuffer.h \
    streamfunction.h \
    realstreamfunction.h \
    datasink.h \
    cosfunction.h
